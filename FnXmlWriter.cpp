//
/////////////////////////////////////////////////////////

#include "Fn.h"
#include "FnXmlWriter.h"

/////////////////////////////////////////////////////////

FnXmlWriter::FnXmlWriter(VariableList *varListInput, DisplayWidget *displayInput)
{

    varList = varListInput;
    display = displayInput;

}

void FnXmlWriter::writeXml(QFile *file)
{

    xmlWriter.setDevice(file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("fn");
    xmlWriter.writeAttribute("version",FnVersion);

    xmlWriter.writeStartElement("variables");
    foreach(QString variableName, varList->keys()) {
        writeVariableEntry(&xmlWriter, variableName, varList->value(variableName));
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("output");
    writeDisplay(&xmlWriter);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

}

/////////////////////////////////////////////////////////
// PRIVATE

void FnXmlWriter::writeVariableEntry(QXmlStreamWriter *xmlWriter, QString &variableName, FnData variable)
{

    xmlWriter->writeStartElement("var");
    switch(variable.type()) {

    case Element:
        xmlWriter->writeAttribute("type","element");
        break;

    case Integer:
        xmlWriter->writeAttribute("type","integer");
        break;

    case Morphism:
        xmlWriter->writeAttribute("type","morphism");
        break;

    default:
        xmlWriter->writeAttribute("type","no type");
        break;

    }

    xmlWriter->writeTextElement("name",variableName);
    xmlWriter->writeTextElement("value",variable.toOutput());
    xmlWriter->writeEndElement();

}

void FnXmlWriter::writeDisplay(QXmlStreamWriter *xmlWriter)
{

    QStringList displayText = display->toPlainText().split(QRegExp("\\[.+\n"));
    foreach(QString displayLine,displayText) {
        xmlWriter->writeTextElement("display",displayLine);
    }

}
