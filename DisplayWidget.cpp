//
/////////////////////////////////////////////////////////

#include <QtGui>

#include "DisplayWidget.h"

const QString DisplayWidget::Input = "in";
const QString DisplayWidget::Output = "out";

/////////////////////////////////////////////////////////

DisplayWidget::DisplayWidget(QWidget *parent) : QTextEdit(parent)
{

  setReadOnly(true);
  setTabChangesFocus(true);
  setOverwriteMode(true);

  setAlignment(Qt::AlignCenter);
  resetLine();

  append(tr("Welcome to the Free Group Calculator!\n"));
  setAlignment(Qt::AlignLeft);

  setFontFamily("Monaco");
  setFontPointSize(12.0);
  append(tr("[%1 %2]:>").arg(Input).arg(lineNumber));
  moveCursor(QTextCursor::End);

}

/////////////////////////////////////////////////////////
//PUBLIC

void DisplayWidget::addLine(const QString &text, const QString &type)
{

  QString arrow_text = text;

  arrow_text.prepend(QString("[%1 %2]:> ").arg(type).arg(lineNumber));

  if (type == Input) {
    moveCursor(QTextCursor::End);
    moveCursor(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
    textCursor().removeSelectedText();
    insertPlainText(arrow_text);
  }

  if (type == Output) {
    append(arrow_text);
    append("");
    lineNumber++;
    append(QString("[%1 %2]:> ").arg(Input).arg(lineNumber));
    moveCursor(QTextCursor::End);
  }

}

/////////////////////////////////////////////////////////
