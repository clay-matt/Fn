// Header file for class Calculator
// inherits QObject

// author Matt Clay
// version 110805

/////////////////////////////////////////////////////////

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QHash>
#include <QStringList>

#include "Fn.h"
#include "Basis.h"
#include "FnData.h"
#include "FnMap.h"
#include "FnWord.h"
#include "FunctionInfo.h"
#include "FunctionInput.h"
#include "WhiteheadData.h"

class QRegExp;

/////////////////////////////////////////////////////////

class Calculator : public QObject {

  Q_OBJECT

public:

    Calculator(QObject *parent = 0);

    VariableList varList() const { return variableList; }

    QString processInput(const QString & input);

    static const QString IdString;
    static const QString PreviousOutput;

private:

    Basis basis;
    FnData previousOutput;
    FunctionInfo presetFunctions;

    // regular expressions
    QRegExp assignmentRegExp;
    QRegExp functionRegExp;
    QRegExp graphRegExp;
    QRegExp listRegExp;
    QRegExp morphismRegExp;
    QRegExp stringRegExp;
    QRegExp variableRegExp;
    QRegExp wordRegExp;

    // lists
    VariableList variableList;

    // processing functions
    FnData compute(const QString &input);
    FunctionInput stringToInput(const QString &input);
    FnData applyFunction(enum FunctionNames fcn, const FunctionInput &input);

    // preset functions
    FnData AbelianizationFunction(const FunctionInput &input);
    FnData BicomponentsFunction(const FunctionInput &input);
    FnData CommutatorFunction(const FunctionInput &input);
    FnData ComposeFunction(const FunctionInput &input);
    FnData ConjugationFunction(const FunctionInput &input);
    FnData ConjugacyProblemFunction(const FunctionInput &input);
    FnData ConnectedComponentsFunction(const FunctionInput &input);
    FnData ExponentiationFunction(const FunctionInput &input);
    FnData IdentityFunction(const FunctionInput &input);
    FnData InverseFunction(const FunctionInput &input);
    FnData IsAutomorphismFunction(const FunctionInput &input);
    FnData IsPrimitiveElementFunction(const FunctionInput &input);
    FnData IsSeparableFunction(const FunctionInput &input);
    FnData IterateFunction(const FunctionInput &input);
    FnData LengthFunction(const FunctionInput &input);
    FnData MapFunction(const FunctionInput &input);
    FnData MultiplyFunction(const FunctionInput &input);
    FnData TwoStepNilpotentFunction(const FunctionInput &input);
    FnData WhiteheadAutomorphismFunction(const FunctionInput &input);
    FnData WhiteheadGraphFunction(const FunctionInput &input);
    FnData WhiteheadProblemFunction(const FunctionInput &input);

signals:
    void updateVariableList(const VariableList &list);

public slots:
    void loadGraph(const QString &name, const FnGraph &Gamma);
    void loadMorphism(const QString &name, const FnMap &phi);
    void resetMorphisms();
    void resetVariables();
    void resetGraphs();

    friend class MainWindow;

};

QStringList breakAtTopLevel(const QString &input);

#endif // CALCULATOR_H
