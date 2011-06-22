// Header file for class FnXmlWriter
// used to write Xml files

// author Matt Clay
// version 110525

/////////////////////////////////////////////////////////

#ifndef FNXMLWRITER_H
#define FNXMLWRITER_H

#include <QFile>
#include <QXmlStreamWriter>

#include "DisplayWidget.h"
#include "FnData.h"

/////////////////////////////////////////////////////////

class FnXmlWriter
{

public:
    FnXmlWriter(VariableList *varListInput, DisplayWidget *displayInput);

    void writeXml(QFile *file);

private:
    QXmlStreamWriter xmlWriter;
    VariableList *varList;
    DisplayWidget *display;

    void writeVariableEntry(QXmlStreamWriter *xmlWriter, QString &variableName, FnData variable);
    void writeDisplay(QXmlStreamWriter *xmlWriter);

};

#endif // FNXMLWRITER_H
