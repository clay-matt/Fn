//
/////////////////////////////////////////////////////////

#include <QtGui>
#include <QWebView>

#include "Calculator.h"
#include "DisplayWidget.h"
#include "FnData.h"
#include "FnWord.h"
#include "FnXmlReader.h"
#include "FnXmlWriter.h"
#include "FunctionListWidget.h"
#include "GraphDialog.h"
#include "GraphListWidget.h"
#include "HelpBrowser.h"
#include "MainWindow.h"
#include "MorphismListWidget.h"
#include "MorphismDialog.h"
#include "ScrollbackLineEdit.h"
#include "VariableListWidget.h"

// define static variables
QStringList MainWindow::recentFiles;

/////////////////////////////////////////////////////////

MainWindow::MainWindow()
{

  // constructor

  // create calculator
  calculator = new Calculator(this);

  // create main window
  display = new DisplayWidget(this);
  setCentralWidget(display);
  display->setFocusPolicy(Qt::ClickFocus);

  // build docks and status bar
  createInputDock();
  createVariableDock();
  createMorphismDock();
  createGraphDock();
  createFunctionDock();
  createStatusBar();

  // build menus
  createActions();

  // read in user settings
  readSettings();

  createMenus();

  // initialize dialogs to 0
  graphDialog = 0;
  morphismDialog = 0;

  // finish up
  setWindowIcon(QIcon(":Images/icon.png"));
  setCurrentFile("");
  setAttribute(Qt::WA_DeleteOnClose);  // used for SDI
  
}

/////////////////////////////////////////////////////////
// PROTECTED

void MainWindow::closeEvent(QCloseEvent *event)
{

  // reimplemented from MainWindow to prompt user to verify the close
  // action and write settings for future sessions

  if (okToContinue()) {
    writeSettings();
    event->accept();
  } else {
    event->ignore();
  }

}

/////////////////////////////////////////////////////////
// PRIVATE

void MainWindow::createInputDock()
{

  // creates input dock

  inputDock = new QDockWidget(tr("input"),this);
  inputEdit = new ScrollbackLineEdit(inputDock);
  inputDock->setWidget(inputEdit);
  inputDock->setMinimumHeight(50);
  inputDock->setContentsMargins(0,0,50,0);
  inputDock->setStatusTip(tr("Enter input"));
  inputDock->setFeatures(QDockWidget::NoDockWidgetFeatures |
                         QDockWidget::DockWidgetVerticalTitleBar);

  addDockWidget(Qt::TopDockWidgetArea,inputDock);
  inputEdit->setFocus(Qt::ActiveWindowFocusReason);

   // add connections
  connect(inputEdit,SIGNAL(returnPressed()),
          this,SLOT(readInput()));

  connect(inputEdit,SIGNAL(returnPressed()),
          this,SLOT(displayModified()));

  connect(inputEdit,SIGNAL(returnPressed()),
          inputEdit,SLOT(clear()));

}

void MainWindow::createVariableDock()
{

  // creates variable dock

  variableDock = new QDockWidget(tr("Variables"),this);
  variableList = new VariableListWidget(calculator->varList(),variableDock);
  variableDock->setWidget(variableList);
  variableDock->setStatusTip(tr("Variable List Dock"));

  addDockWidget(Qt::RightDockWidgetArea, variableDock);

  // add connections
  connect(calculator,
          SIGNAL(updateVariableList(const VariableList &)),
          variableList,
          SLOT(updateList(const VariableList &)));

  connect(variableList,SIGNAL(echoValue(const QString &)),
          inputEdit,SLOT(addToLine(const QString &)));

}

void MainWindow::createMorphismDock()
{

  // create morphism dock

  morphismDock = new QDockWidget(tr("Morphisms"),this);
  morphismList = new MorphismListWidget(calculator->varList(),morphismDock);
  morphismDock->setWidget(morphismList);
  morphismDock->setStatusTip(tr("Morphism List Dock"));

  addDockWidget(Qt::RightDockWidgetArea, morphismDock);

  // add connections
  connect(calculator,
          SIGNAL(updateVariableList(const VariableList &)),
          morphismList,
          SLOT(updateList(const VariableList &)));

  connect(morphismList,SIGNAL(echoValue(const QString &)),
          inputEdit,SLOT(addToLine(const QString &)));

}

void MainWindow::createGraphDock()
{

    graphDock = new QDockWidget(tr("Graphs"),this);
    graphList = new GraphListWidget(calculator->varList(),graphDock);
    graphDock->setWidget(graphList);
    graphDock->setStatusTip(tr("Graph List Dock"));

    addDockWidget(Qt::RightDockWidgetArea, graphDock);

    // add connections
    connect(calculator,
            SIGNAL(updateVariableList(const VariableList &)),
            graphList,
            SLOT(updateList(const VariableList &)));

    connect(graphList,SIGNAL(echoValue(const QString &)),
            inputEdit,SLOT(addToLine(const QString &)));

}

void MainWindow::createFunctionDock()
{

  functionDock = new QDockWidget(tr("Functions"),this);
  functionList = new FunctionListWidget(functionDock);
  functionDock->setWidget(functionList);
  functionDock->setStatusTip(tr("Function List Dock"));

  addDockWidget(Qt::RightDockWidgetArea, functionDock);

  // add connections
  connect(functionList,SIGNAL(functionSkeleton(const QString &)),
          inputEdit,SLOT(addToLine(const QString &)));

}

void MainWindow::createActions()
{

  // creates actions for menus

  // file actions
  newAction = new QAction(tr("&New"), this);
  //newAction->setIcon(QIcon(""));
  newAction->setShortcut(QKeySequence::New);
  newAction->setStatusTip(tr("Create a new worksheet"));
  connect(newAction,SIGNAL(triggered()), 
	  this,SLOT(newFile()));

  openAction = new QAction(tr("&Open.."), this);
  //openAction->setIcon(QIcon(""));
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open an existing file"));
  connect(openAction,SIGNAL(triggered()), 
	  this,SLOT(open()));

  saveAction = new QAction(tr("&Save"), this);
  //saveAction->setIcon(QIcon(""));
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save current file"));
  connect(saveAction,SIGNAL(triggered()), 
	  this,SLOT(save()));

  saveAsAction = new QAction(tr("Save &As.."), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setStatusTip(tr("Save current file as"));
  connect(saveAsAction,SIGNAL(triggered()), 
	  this,SLOT(saveAs()));

  for (int i = 0; i < MaxRecentFiles; ++i) {
    recentFileActions[i] = new QAction(this);
    recentFileActions[i]->setVisible(false);
    connect(recentFileActions[i],SIGNAL(triggered()),
	    this,SLOT(openRecentFile()));
  }

  closeAction = new QAction(tr("Close &Window"),this);
  closeAction->setShortcut(QKeySequence::Close);
  closeAction->setStatusTip(tr("Close the current window"));
  connect(closeAction,SIGNAL(triggered()),
	  this,SLOT(close()));

  // edit actions
  cutAction = new QAction(tr("Cu&t"),this);
  //cutAction->setIcon(QIcon(""));
  cutAction->setShortcut(QKeySequence::Cut);
  cutAction->setStatusTip(tr("Cut selected text"));
  connect(cutAction,SIGNAL(triggered()),
        this,SLOT(cut()));

  copyAction = new QAction(tr("&Copy"),this);
  //copyAction->setIcon(QIcon(""));
  copyAction->setShortcut(QKeySequence::Copy);
  copyAction->setStatusTip(tr("Copy selected text"));
  connect(copyAction,SIGNAL(triggered()),
          this,SLOT(copy()));

  pasteAction = new QAction(tr("&Paste"),this);
  //pasteAction->setIcon(QIcon(""));
  pasteAction->setShortcut(QKeySequence::Paste);
  pasteAction->setStatusTip(tr("Paste from clipboard"));
  connect(pasteAction,SIGNAL(triggered()),
        this,SLOT(paste()));

  selectAllAction = new QAction(tr("&Select All"),this);
  //selectAllAction->setIcon(QIcon(""));
  selectAllAction->setShortcut(QKeySequence::SelectAll);
  selectAllAction->setStatusTip(tr("Select all text"));
  connect(selectAllAction,SIGNAL(triggered()),
        this,SLOT(selectAll()));

  // tools actions
  clearVariablesAction = new QAction(tr("Clear &Variables"),this);
  clearVariablesAction->setStatusTip(tr("Clear Variables List"));
  connect(clearVariablesAction,SIGNAL(triggered()),
	  calculator,SLOT(resetVariables()));

  clearMorphismAction = new QAction(tr("Clear &Morphisms"),this);
  clearMorphismAction->setStatusTip(tr("Clear Morphism List"));
  connect(clearMorphismAction,SIGNAL(triggered()),
          calculator,SLOT(resetMorphisms()));

  clearGraphAction = new QAction(tr("Clear &Graphs"),this);
  clearGraphAction->setStatusTip(tr("Clear Graph List"));
  connect(clearMorphismAction,SIGNAL(triggered()),
          calculator,SLOT(resetGraphs()));

  graphAction = new QAction(tr("Load &Graph"),this);
  graphAction->setShortcut(Qt::CTRL + Qt::Key_G);
  graphAction->setStatusTip(tr("Load a Graph using dialog"));
  connect(graphAction,SIGNAL(triggered()),
          this,SLOT(loadGraph()));

  morphismAction = new QAction(tr("&Load Morphism"),this);
  morphismAction->setShortcut(Qt::CTRL + Qt::Key_L);
  morphismAction->setStatusTip(tr("Load a Morphism using dialog"));
  connect(morphismAction,SIGNAL(triggered()),
          this,SLOT(loadMorphism()));

  // options actions
  statusAction = new QAction(tr("Show &Status Bar"),this);
  statusAction->setCheckable(true);
  statusAction->setChecked(showStatus);
  statusAction->setStatusTip(tr("Show or hide the status bar"));
  connect(statusAction,SIGNAL(toggled(bool)),
	  this,SLOT(showStatusBar(bool)));

  variableListAction = variableDock->toggleViewAction();
  variableListAction->setText(tr("Show &Variable List"));
  variableListAction->setStatusTip(tr("Show or hide the variable list"));
  // connections are automatically made

  morphismListAction = morphismDock->toggleViewAction();
  morphismListAction->setText(tr("Show &Morphism List"));
  morphismListAction->setStatusTip(tr("Show or hide the morphism list"));
  // connections are automatically made

  graphListAction = graphDock->toggleViewAction();
  graphListAction->setText(tr("Show &Graph List"));
  graphListAction->setStatusTip(tr("Show or hide the graph list"));
  // connections are automatically made

  functionListAction = functionDock->toggleViewAction();
  functionListAction->setText("Show &Function List");
  functionListAction->setStatusTip(tr("Show or hide the function list"));
  // connections are automatically made

  // help actions
  helpAction = new QAction(tr("&Help"),this);
  helpAction->setStatusTip(tr("Show the Help browser"));
  connect(helpAction,SIGNAL(triggered()),
          this,SLOT(help()));

  aboutAction = new QAction(tr("&About"),this);
  aboutAction->setStatusTip(tr("Show the About box"));
  connect(aboutAction,SIGNAL(triggered()),
	  this,SLOT(about()));

  aboutQtAction = new QAction(tr("About &Qt"),this);
  aboutAction->setStatusTip(tr("Show the Qt Library's About box"));
  connect(aboutQtAction,SIGNAL(triggered()),
	  qApp,SLOT(aboutQt()));

}

void MainWindow::createMenus()
{

  // creates menus

  // file menu
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);

  openRecentSubMenu = fileMenu->addMenu(tr("Open &Recent"));
  for (int i = 0; i < MaxRecentFiles; ++i)
    openRecentSubMenu->addAction(recentFileActions[i]);

  fileMenu->addSeparator();
  fileMenu->addAction(closeAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);

  // edit menu
  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAction);
  editMenu->addAction(copyAction);
  editMenu->addAction(pasteAction);
  editMenu->addSeparator();
  editMenu->addAction(selectAllAction);

  // tools menu
  toolsMenu = menuBar()->addMenu(tr("&Tools"));
  toolsMenu->addAction(clearVariablesAction);
  toolsMenu->addAction(clearMorphismAction);
  toolsMenu->addAction(clearGraphAction);
  toolsMenu->addSeparator();
  toolsMenu->addAction(graphAction);
  toolsMenu->addAction(morphismAction);

  // options menu
  optionsMenu = menuBar()->addMenu(tr("&Options"));
  optionsMenu->addAction(statusAction);
  optionsMenu->addAction(variableListAction);
  optionsMenu->addAction(morphismListAction);
  optionsMenu->addAction(functionListAction);
  optionsMenu->addAction(graphListAction);

  menuBar()->addSeparator();

  // help menu
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpAction);
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);

}

void MainWindow::createStatusBar()
{

  // creates status bar

  statusBar()->showMessage(tr("Ready"));
  showStatusBar(showStatus);
}

void MainWindow::readSettings()
{

  // reads user settings

  QSettings settings("Matt Clay", "Worksheet");

  restoreGeometry(settings.value("geometry").toByteArray());

  recentFiles = settings.value("recentFiles").toStringList();
  foreach (QWidget *win, QApplication::topLevelWidgets()) {
    if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
      mainWin->updateRecentFileActions();
  }

  showStatus = settings.value("showStatus", false).toBool();
  statusAction->setChecked(showStatus);
  showStatusBar(showStatus);

}

void MainWindow::writeSettings()
{

  // writes current settings

  QSettings settings("Matt Clay", "Worksheet");

  settings.setValue("geometry", saveGeometry());
  settings.setValue("recentFiles", recentFiles);
  settings.setValue("showStatus", showStatus);

}

bool MainWindow::okToContinue()
{

  if(isWindowModified()) {
    int r = QMessageBox::warning(this, tr("Free Group Calculator"),
				 tr("The file has been modified.\n"
				    "Do you want to save your changes?"),
				 QMessageBox::Yes | QMessageBox::No
				 | QMessageBox::Cancel);

    if (r == QMessageBox::Yes) {
      return save();
    } else if (r == QMessageBox::Cancel) {
      return false;
    }

  }

  return true;

}

bool MainWindow::loadFile(const QString &fileName)
{

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
      statusBar()->showMessage(tr("Loading canceled"), 2000);
      QMessageBox::warning(this, tr("Free Group Calculator"),
                          tr("Cannot read file %1:\n%2")
                          .arg(file.fileName())
                          .arg(file.errorString()));
      return false;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  FnXmlReader reader(&calculator->variableList,display);
  reader.readXml(&file);

  if(reader.hasError()) {
      QApplication::restoreOverrideCursor();
      statusBar()->showMessage(tr("Loading canceled"), 2000);
      QMessageBox::warning(this, tr("Free Group Calculator"),
                          tr("Failed to parse file %1:\n%2")
                          .arg(file.fileName())
                          .arg(reader.errorString()));
      return false;
  }

  file.close();
  if(file.error()) {
      QApplication::restoreOverrideCursor();
      statusBar()->showMessage(tr("Loading canceled"), 2000);
      QMessageBox::warning(this, tr("Free Group Calculator"),
                          tr("Cannot read file %1:\n%2")
                          .arg(file.fileName())
                          .arg(file.errorString()));
      return false;
  }

  // load variables
  variableList->updateList(calculator->varList());
  morphismList->updateList(calculator->varList());
  QApplication::restoreOverrideCursor();
  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
  return true;

}

bool MainWindow::saveFile(const QString &fileName)
{

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) {
      statusBar()->showMessage(tr("Saving canceled"), 2000);
      QMessageBox::warning(this, tr("Free Group Calculator"),
                          tr("Cannot write file %1:\n%2")
                          .arg(file.fileName())
                          .arg(file.errorString()));
      return false;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  // get variable list
  VariableList varList = calculator->varList();

  FnXmlWriter writer(&varList,display);
  writer.writeXml(&file);

  file.close();
  if(file.error()) {
      statusBar()->showMessage(tr("Saving canceled"), 2000);
      QMessageBox::warning(this, tr("Free Group Calculator"),
                          tr("Cannot write file %1:\n%2")
                          .arg(file.fileName())
                          .arg(file.errorString()));
      return false;
  }

  QApplication::restoreOverrideCursor();
  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;

}

void MainWindow::setCurrentFile(const QString &fileName)
{

  curFile = fileName;
  setWindowModified(false);
  QString shownName = tr("Untitled");

  if (!curFile.isEmpty()) {
    shownName = strippedName(curFile);
    recentFiles.removeAll(curFile);
    recentFiles.prepend(curFile);

    foreach (QWidget *win, QApplication::topLevelWidgets()) {
      if (MainWindow *mainWin = qobject_cast<MainWindow *>(win))
	mainWin->updateRecentFileActions();
    }
  }

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Worksheet")));

}

void MainWindow::updateRecentFileActions()
{

  QMutableStringListIterator i(recentFiles);
  
  while(i.hasNext()) {
    if (!QFile::exists(i.next()))
      i.remove();
  }

  for (int j = 0; j < MaxRecentFiles; ++j) {
    if (j < recentFiles.count()) {
      QString text = strippedName(recentFiles[j]);
      recentFileActions[j]->setText(text);
      recentFileActions[j]->setData(recentFiles[j]);
      recentFileActions[j]->setVisible(true);
    } else {
      recentFileActions[j]->setVisible(false);
    }
  }    

}

QString MainWindow::strippedName(const QString &fullFileName)
{

  return QFileInfo(fullFileName).fileName();

}

/////////////////////////////////////////////////////////
// SLOTS

void MainWindow::about()
{

  // About box for Free Group Calculator

  QMessageBox::about(this,tr("About Free Group Calculator"),
                     tr("<h2>Free Group Calculator %1</h2>"
                        "<p>Last Update %2</p>"
                        "<p>author: Matt Clay</p>"
                        "<p>email: <a href=\"mailto:mclay@allegheny.edu\">mclay@allegheny.edu</a></p>")
                     .arg(FnVersion)
                     .arg(updateDate));

}

void MainWindow::copy()
{

    // get the last child widget that has focus

    // try to cast it as line edit
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(focusWidget());
    if (lineEdit) lineEdit->copy( );

    // try to cast it as text edit
    QTextEdit *textEdit = dynamic_cast<QTextEdit*>(focusWidget());
    if (textEdit) textEdit->copy();

}

void MainWindow::cut()
{

    // get the last child widget that has focus

    // try to cast it as line edit
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(focusWidget());
    if (lineEdit) lineEdit->cut();

}

void MainWindow::displayModified()
{

  setWindowModified(true);

}

void MainWindow::help()
{

    HelpBrowser::showPage("qrc:/Documentation/index.html");

}

void MainWindow::loadGraph() {

    graphDialog = new GraphDialog(this);

    // add connections
    connect(graphDialog,SIGNAL(updateGraphList(const QString &, const FnGraph &)),
              calculator,SLOT(loadGraph(const QString &, const FnGraph &)));

    connect(graphDialog,SIGNAL(updateGraphList(const QString &, const FnGraph &)),
            this,SLOT(displayModified()));


    graphDialog->show();
    graphDialog->raise();
    graphDialog->activateWindow();

}

void MainWindow::loadMorphism()
{

  morphismDialog = new MorphismDialog(this);

  // add connections
  connect(morphismDialog,SIGNAL(updateMorphismList(const QString &, const FnMap &)),
            calculator,SLOT(loadMorphism(const QString &, const FnMap &)));

  connect(morphismDialog,SIGNAL(updateMorphismList(const QString &, const FnMap &)),
          this,SLOT(displayModified()));

  morphismDialog->show();
  morphismDialog->raise();
  morphismDialog->activateWindow();

}

void MainWindow::newFile()
{

  // creates new worksheet in a new window

  MainWindow *mainWin = new MainWindow;
  mainWin->show();

}

void MainWindow::open()
{

  // opens existing file

  if (okToContinue()) {

    QString fileName = QFileDialog::getOpenFileName(this,
			                     tr("Open Worksheet"),".",
					     tr("Free Group Worksheets (*.fg)"));
    if (!fileName.isEmpty())
      loadFile(fileName);
  }

}

void MainWindow::openRecentFile()
{

  if (okToContinue()) {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
      loadFile(action->data().toString());
  }

}

void MainWindow::paste()
{

    // get the last child widget that has focus

    // try to cast it as line edit
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(focusWidget());
    if (lineEdit) lineEdit->paste();

}

bool  MainWindow::save()
{

  // saves existing file

  if (curFile.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(curFile);
  }

}

bool  MainWindow::saveAs()
{

  // save existing file as

  QString fileName = QFileDialog::getSaveFileName(this,
                                          tr("Save Worksheet"),".",
                                         tr("Free Group Worksheets (*.fg)"));

  if (fileName.isEmpty())
    return false; 

  return saveFile(fileName);

}

void MainWindow::selectAll()
{

    // get the last child widget that has focus

    // try to cast it as line edit
    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(focusWidget());
    if (lineEdit) lineEdit->selectAll();

}

void MainWindow::showStatusBar(bool toShow)
{

  // toggles visibilty of status bar

  if (toShow == true) setStatusBar(statusBar());
  if (toShow == false) setStatusBar(0);

  // set settings
  showStatus = toShow;

}

void MainWindow::readInput()
{

  // this is where the main window interacts with the calculator

  QString inText(inputEdit->text().trimmed());
  display->addLine(inText,DisplayWidget::Input);

  QString outText(calculator->processInput(inText));
  if (!outText.isEmpty())
    display->addLine(outText,DisplayWidget::Output);
  
}
