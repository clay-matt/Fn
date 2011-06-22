// Header file for class MorphismListWidget
// inherits QTableWidget

// author Matt Clay
// version 100723

/////////////////////////////////////////////////////////

#ifndef MORPHISMLISTWIDGET_H
#define MORPHISMLISTWIDGET_H

#include <QHash>
#include <QTableWidget>

#include "FnData.h"

/////////////////////////////////////////////////////////

class MorphismListWidget : public QTableWidget
{

  Q_OBJECT

public:
  MorphismListWidget(const VariableList &variableList, QWidget *parent = 0);

public slots:
 void updateList(const VariableList &variableList);

private slots:
 void itemValue(QTableWidgetItem *item);

signals:
 void echoValue(const QString &value);


};

/////////////////////////////////////////////////////////

#endif // MORPHISMLISTWIDGET_H
