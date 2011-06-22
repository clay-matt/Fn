// Header file for class ScrollbackLineEdit
// inherits QLineEdit

// author Matt Clay
// version 110215

/////////////////////////////////////////////////////////

#ifndef SCROLLBACKLINEEDIT_H
#define SCROLLBACKLINEEDIT_H

#include <QLineEdit>

class QKeyEvent;
class QStringList;

/////////////////////////////////////////////////////////

class ScrollbackLineEdit : public QLineEdit
{

  Q_OBJECT

public:
  ScrollbackLineEdit(QWidget *parent = 0);

private:
  int row;
  QStringList previousEntries;

protected:
  void keyPressEvent(QKeyEvent *event);

private slots:
  void takeEntry();

public slots:
  void addToLine(const QString &str);

};

/////////////////////////////////////////////////////////

#endif // SCROLLBACKLINEEDIT_H
