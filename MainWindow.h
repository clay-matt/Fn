// Header file for class MainWindow
// inherits MainWindow

// author Matt Clay
// version 110608

/////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>

#include "Fn.h"

class Calculator;
class DisplayWidget;
class FunctionListWidget;
class GraphDialog;
class GraphListWidget;
class MorphismDialog;
class MorphismListWidget;
class QAction;
class QDockWidget;
class QLabel;
class QPushButton;
class QToolBar;
class ScrollbackLineEdit;
class VariableListWidget;

/////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{

  Q_OBJECT

public:
  MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void about();
  void copy();
  void cut();
  void displayModified();
  void help();
  void loadGraph();
  void loadMorphism();
  void newFile();
  void open();
  void openRecentFile();
  void paste();
  bool save();
  bool saveAs();
  void selectAll();
  void showStatusBar(bool toShow);

  void readInput();

private:
  // variables
  bool showStatus;
  static QStringList recentFiles;
  QString curFile;
  enum {MaxRecentFiles = 5};

  Calculator *calculator;
  DisplayWidget *display;

  // docks
  QDockWidget *inputDock; // non-movable
  ScrollbackLineEdit *inputEdit;
  QDockWidget *variableDock;
  VariableListWidget *variableList;
  QDockWidget *morphismDock;
  MorphismListWidget *morphismList;
  QDockWidget *functionDock;
  FunctionListWidget *functionList;
  QDockWidget *graphDock;
  GraphListWidget *graphList;
  
  // dialogs
  GraphDialog *graphDialog;
  MorphismDialog *morphismDialog;

  // menus
  QMenu *fileMenu;
  QMenu *openRecentSubMenu;
  QMenu *editMenu;
  QMenu *toolsMenu;
  QMenu *optionsMenu;
  QMenu *helpMenu;

  // file actions
  QAction *newAction;
  QAction *openAction;
  QAction *saveAction;
  QAction *saveAsAction;
  QAction *closeAction;
  QAction *recentFileActions[MaxRecentFiles];

  // edit actions
  QAction *cutAction;
  QAction *copyAction;
  QAction *pasteAction;
  QAction *selectAllAction;

  // tools actions
  QAction *clearVariablesAction;
  QAction *clearMorphismAction;
  QAction *clearGraphAction;
  QAction *graphAction;
  QAction *morphismAction;

  // options actions
  QAction *statusAction;
  QAction *variableListAction;
  QAction *morphismListAction;
  QAction *graphListAction;
  QAction *functionListAction;

  // help actions
  QAction *helpAction;
  QAction *aboutAction;
  QAction *aboutQtAction;

  // build functions
  void createInputDock();
  void createVariableDock();
  void createMorphismDock();
  void createGraphDock();
  void createFunctionDock();
  void createActions();
  void createMenus();
  void createStatusBar();

  // settings functions
  void readSettings();
  void writeSettings();

  // file functions
  bool okToContinue();
  bool loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  void updateRecentFileActions();
  QString strippedName(const QString &fullFileName);

};

/////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
