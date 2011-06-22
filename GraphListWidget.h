// Header file for class GraphListWidget
// inherits QTableWidget

// author Matt Clay
// version 110608

/////////////////////////////////////////////////////////

#ifndef GRAPHLISTWIDGET_H
#define GRAPHLISTWIDGET_H

#include <QHash>
#include <QTableWidget>

#include "FnData.h"

/////////////////////////////////////////////////////////


class GraphListWidget : public QTableWidget
{

    Q_OBJECT

public:
    GraphListWidget(const VariableList &variableList, QWidget *parent = 0);

public slots:
    void updateList(const VariableList &variableList);

private slots:
    void itemValue(QTableWidgetItem *item);

signals:
 void echoValue(const QString &value);

};

#endif // GRAPHLISTWIDGET_H
