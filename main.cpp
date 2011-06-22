/////////////////////////////////////////////////////////

// Free Group Calculator
// author Matt Clay
// version 110222

// This is the main file for application.

/////////////////////////////////////////////////////////

#include <QtGui>
#include <QApplication>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "MainWindow.h"

/////////////////////////////////////////////////////////

void loadStyleSheet()
{

  // loads the style sheet for the application
  // code snippet copied from:
  // http://wiki.forum.nokia.com/index.php/CS001502_-_Applying_a_Qt_style_sheet_to_an_application_via_QApplication

  // opens file style.qss from top directory
  QFile data(":/style.qss");

// read in file
  if(data.open(QFile::ReadOnly)) {
    QTextStream styleIn(&data);
    QString style = styleIn.readAll();
    data.close();
    // qApp is a global variable for the application
    qApp->setStyleSheet(style);
  }

}

int main(int argc, char *argv[])
{

  // code copied from page 71:
  // C++ Programming with Qt 4 (2nd Edition)
  // Jasmin Blanchette & Mark Summerfield
  // Prentice Hall, 2008

  // This code creates an SDI application.
  QApplication app(argc,argv);
  MainWindow *mainWin = new MainWindow;
  loadStyleSheet();
  mainWin->show();
  return app.exec();

}

/////////////////////////////////////////////////////////
