// Header file for class FunctionInfo
// inherits QObject

// author Matt Clay
// version 120514

/////////////////////////////////////////////////////////

#ifndef FUNCTIONINFO_H // avoid double includes
#define FUNCTIONINFO_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "Fn.h"

/////////////////////////////////////////////////////////

class FunctionInfo : public QObject
{

    Q_OBJECT

    public:
    FunctionInfo(QObject *parent = 0);

    QStringList functions() const {
      return functionEnum.keys();
    }
    enum FunctionNames fcnTag(const QString & fcn) const {
      return functionEnum.value(fcn,ERROR);
    }
    QString fcnName(enum FunctionNames fcn) const {
      return functionName.value(fcn);
    }
    QHash<enum FunctionNames, QString> fcnNames() const {
      return functionName;
    }
    QString fcnSkeleton(enum FunctionNames fcn) const {
      return functionSkeleton.value(fcn);
    }
    QString fcnStatusTip(enum FunctionNames fcn) const {
      return functionStatusTip.value(fcn);
    }
    QList<enum VariableTypes> fcnInput(enum FunctionNames fcn) const {
      return functionInput.value(fcn);
    }
    int fcnNVariables(enum FunctionNames fcn) const {
      return functionInput.value(fcn).size();
    }
    enum VariableTypes fcnOutput(enum FunctionNames fcn) const {
      return functionOutput.value(fcn,NoType);
    }

    private:
    QHash<QString, enum FunctionNames> functionEnum;
    QHash<enum FunctionNames, QString> functionName;
    QHash<enum FunctionNames, QString> functionSkeleton;
    QHash<enum FunctionNames, QString> functionStatusTip;
    QHash<enum FunctionNames, QList<enum VariableTypes> > functionInput;
    QHash<enum FunctionNames, enum VariableTypes> functionOutput;

};

/////////////////////////////////////////////////////////

#endif // FUNCTIONINFO_H
