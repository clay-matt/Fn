// Header file for class Display
// inherits QTextEdit

// author Matt Clay
// version 110524

/////////////////////////////////////////////////////////

#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QTextEdit>


/////////////////////////////////////////////////////////

class DisplayWidget : public QTextEdit
{

    Q_OBJECT

private:
    int lineNumber;

public:
    static const QString Input;
    static const QString Output;

    DisplayWidget(QWidget *parent = 0);

    void resetLine() { lineNumber = 1; }
    void addLine(const QString &text, const QString &type);

};

/////////////////////////////////////////////////////////

#endif // DISPLAYWIDGET_H
