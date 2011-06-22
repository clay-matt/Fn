//

#include <QtGui>

#include "ScrollbackLineEdit.h"

/////////////////////////////////////////////////////////

ScrollbackLineEdit::ScrollbackLineEdit(QWidget *parent) : QLineEdit(parent)
{

  // constructor
  row = 0;

  // add connections
  connect(this,SIGNAL(returnPressed()),
	  this,SLOT(takeEntry()));

}

/////////////////////////////////////////////////////////
// PROTECTED

void ScrollbackLineEdit::keyPressEvent(QKeyEvent *event) 
{

  if (event->key() == Qt::Key_Up && 0 < row && row <= previousEntries.size()) {

      row--;
      clear();
      insert(previousEntries.at(row));
   
  }

  if (event->key() == Qt::Key_Down) {

    if (row > previousEntries.size() - 1) QApplication::beep();

    if (row == previousEntries.size() - 1) {

      row++;
      clear();

    }

    if (-1 <= row && row < previousEntries.size() - 1) {

      row++;
      clear();
      insert(previousEntries.at(row));

    }
    
  }

  QLineEdit::keyPressEvent(event);

}

/////////////////////////////////////////////////////////
// PRIVATE SLOTS

void ScrollbackLineEdit::takeEntry()
{

  QString entry = text();

  // record if entry contains at least one non-whitespace character
  if (entry.contains(QRegExp("[^\\s]"))) {
    
    previousEntries.append(entry);
    row = previousEntries.size();

  }

}

/////////////////////////////////////////////////////////
// PUBLIC SLOTS

void ScrollbackLineEdit::addToLine(const QString &str)
{

  setText(text() + str);
  setFocus(Qt::ActiveWindowFocusReason);

}
