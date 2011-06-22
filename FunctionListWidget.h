// Header file for class FunctionListWidget
// inherits QListWidget

// author Matt Clay
// version 110216

/////////////////////////////////////////////////////////

#ifndef FUNCTIONLISTWIDGET_H
#define FUNCTIONLISTWIDGET_H

#include <QHash>
#include <QListWidget>

#include "FunctionInfo.h"

class QString;

/////////////////////////////////////////////////////////

class FunctionListWidget : public QListWidget
{

    Q_OBJECT

    public:
    FunctionListWidget(QWidget *parent = 0);

    private:
    FunctionInfo presetFunctions;

    private slots:
    void itemSkeleton(QListWidgetItem *item);

    signals:
    void functionSkeleton(const QString & functionSkeleton);

};

/////////////////////////////////////////////////////////

#endif // FUNCTIONLISTWIDGET_H
