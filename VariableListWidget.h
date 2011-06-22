// Header file for class VariableListWidget
// inherits QTableWidget

// author Matt Clay
// version 110215

/////////////////////////////////////////////////////////

#ifndef VARIABLELISTWIDGET_H
#define VARIABLELISTWIDGET_H

#include <QHash>
#include <QTableWidget>

#include "FnData.h"

class QString;

/////////////////////////////////////////////////////////

class VariableListWidget : public QTableWidget
{

 Q_OBJECT

public:
 VariableListWidget(const VariableList & variableList,
                QWidget *parent = 0);

public slots:
 void updateList(const VariableList & variableList);

private slots:
 void itemValue(QTableWidgetItem *item);

signals:
 void echoValue(const QString & value);

};

/////////////////////////////////////////////////////////

#endif // VARIABLELISTWIDGET_H
