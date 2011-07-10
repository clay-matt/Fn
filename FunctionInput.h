// Header file for class FunctionInput
// inherits QList<FnData>

// author Matt Clay
// version 110707

/////////////////////////////////////////////////////////

#ifndef FUNCTIONINPUT_H // aoud double incldues
#define FUNCTIONINPUT_H

#include <QList>

#include "Fn.h"
#include "FnData.h"

/////////////////////////////////////////////////////////

class FunctionInput : public QList<FnData>
{

    public:
    FunctionInput();

    bool isAcceptable(const QList<enum VariableTypes> & types) const;
    QList<enum VariableTypes> varTypes() const;

    private:
    enum VariableTypes optional(const enum VariableTypes type) const;

};

typedef QListIterator<FnData> FunctionInputIterator;
typedef QMutableListIterator<FnData> FunctionInputMutableIterator;

/////////////////////////////////////////////////////////

#endif // FUNCTIONINPUT_H
