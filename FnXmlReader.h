// Header file for class FnXmlReader
// used to read xml files

// author Matt Clay
// version 110627

/////////////////////////////////////////////////////////

#ifndef FNXMLREADER_H
#define FNXMLREADER_H

#include <QFile>
#include <QXmlStreamReader>

#include "DisplayWidget.h"
#include "FnData.h"

class FnXmlReader
{

public:
    FnXmlReader(VariableList *varListInput, DisplayWidget *displayInput);

    void readXml(QFile *file);
    bool hasError();
    QString errorString();

private:
    QXmlStreamReader reader;
    VariableList *varList;
    DisplayWidget *display;

    void readCalculatorElement();
    void readVariableListElement();
    void readVariableElement();
    void skipUnknownElement();

    FnData processData(const QString & varValue, const QString & varType);

};

#endif // FNXMLREADER_H
