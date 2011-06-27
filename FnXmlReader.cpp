//
/////////////////////////////////////////////////////////

#include "Calculator.h"
#include "Fn.h"
#include "FnData.h"
#include "FnXmlReader.h"

/////////////////////////////////////////////////////////

FnXmlReader::FnXmlReader(VariableList *varListInput, DisplayWidget *displayInput)
{

    varList = varListInput;
    display = displayInput;

}

void FnXmlReader::readXml(QFile *file)
{

    reader.setDevice(file);

    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isStartElement()) {
            if(reader.name() == "fn") { // check version number
                QString fileVersion = reader.attributes().value("version").toString();
                bool ok;
                if(FnVersion.toFloat() < fileVersion.toFloat(&ok) || !ok ) {
                    reader.raiseError(QObject::tr("File version (%1) is more recent than current version (%2)")
                                      .arg(fileVersion).arg(FnVersion));
                } else {
                readCalculatorElement();
                }
            } else {
                reader.raiseError(QObject::tr("Not a Free Group Calculator File"));
            }
        } else {
            reader.readNext();
        }
    }

}

bool FnXmlReader::hasError()
{

    return reader.hasError();

}

QString FnXmlReader::errorString()
{

    return reader.errorString();

}

/////////////////////////////////////////////////////////
// PRIVATE

void FnXmlReader::readCalculatorElement()
{

    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) { // can only be </fn>
            reader.readNext();
            break;
        }

        if(reader.isStartElement()) {
            if(reader.name() == "variables") {
                readVariableListElement();
            } else if(reader.name() == "output") {
                skipUnknownElement(); // need to make reader here!
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }

}

void FnXmlReader::readVariableElement()
{

    QString varType = reader.attributes().value("type").toString();
    QString varName;
    QString varValue;
    bool readName = false;
    bool readValue = false;

    reader.readNext();
    if(varType == "no type" || varType.isEmpty()) {
        reader.raiseError(QObject::tr("Variable error: type not specified"));
        return;
    }

    while(!reader.atEnd()) {
        if(reader.isEndElement()) { // can only be </var>
            reader.readNext();
            break;
        }

        if(reader.isStartElement()) {
            if(reader.name() == "name") {
                if(readName) { // already read in name of variable
                    reader.raiseError(QObject::tr("Variable Error: duplicate name field"));
                }
                else {
                    varName = reader.readElementText();
                    readName = true;
                    reader.readNext();
                }
            } else if(reader.name() == "value") {
                if(readValue) { // already read in value of variable
                    reader.raiseError(QObject::tr("Variable Error: duplicate value field"));
                }
                else {
                    varValue = reader.readElementText();
                    readValue = true;
                    reader.readNext();
                }
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }

    }

    if(!readName || !readValue) {
        reader.raiseError(QObject::tr("Variable Error: name or value field missing"));
        return;
    }

    FnData variable;

    if(varType == "element"){
        FnWord u(varValue);
        if(!u.checkBasis()) {
            reader.raiseError(QObject::tr("Variable Error: %1 is not a valid element").arg(varValue));
            return;
        }
        variable.setElement(u);
    }

    else if(varType == "integer") {
        int i;
        bool ok;
        i = varValue.toInt(&ok);
        if(!ok) {
            reader.raiseError(QObject::tr("Variable Error: %1 is not a valid integer").arg(varValue));
            return;
        }
        variable.setInteger(i);
    }

    else if(varType == "graph") {
        if(!varValue.startsWith(QChar('(')) || !varValue.endsWith(QChar(')'))) {
            reader.raiseError(QObject::tr("Variable Error: %1 is not a valid graph").arg(varValue));
            return;
        }

        variable = loadGraphData(varValue);

        if(variable.type() == FailMessage) {
            reader.raiseError(variable.toOutput());
            return;
        }

    }

    else if(varType == "morphism") {
        if(!varValue.startsWith(QChar('{')) || !varValue.endsWith(QChar('}'))) {
            reader.raiseError(QObject::tr("Variable Error: %1 is not a valid morphism").arg(varValue));
            return;
        }

        variable = loadMorphismData(varValue);

        if(variable.type() == FailMessage) {
            reader.raiseError(variable.toOutput());
            return;
        }

    }

    else {
        reader.raiseError(QObject::tr("Variable Error: cannot identify type"));
        return;
    }

    varList->insert(varName,variable);

}

void FnXmlReader::readVariableListElement()
{

    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) { // can only be </variables>
            reader.readNext();
            break;
        }

        if(reader.isStartElement()) {
            if(reader.name() == "var") {
                readVariableElement();
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }

}

void FnXmlReader::skipUnknownElement()
{

    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }

}
