//
/////////////////////////////////////////////////////////

#include "Basis.h"
#include "FnData.h"

/////////////////////////////////////////////////////////

// CONSTRUCTORS
FnData::FnData()
{

  data_type = NoType;
  failMessage = "Unknown data type encountered for FnData.";

}

FnData::FnData(int i)
{

  data_type = Integer;
  i_value = i;

}

FnData::FnData(const FnGraph &Gamma)
{

    data_type = Graph;
    g_value = Gamma;

}

FnData::FnData(const FnMap &phi)
{

  data_type = Morphism;
  f_value = phi;

}

FnData::FnData(const FnWord &u)
{

  data_type = Element;
  u_value = u;

}

FnData::FnData(const QList<FnGraph> &list)
{

    data_type = GraphList;
    g_list = list;

}

FnData::FnData(const QList<QString> &list)
{

    data_type = StringList;
    s_list = list;

}

/////////////////////////////////////////////////////////

int FnData::integerData() const
{

  return i_value;

}

FnGraph FnData::graphData() const
{

    return g_value;

}

FnMap FnData::mapData() const
{

    return f_value;

}

FnWord FnData::wordData() const
{

  return u_value;

}

QList<FnGraph> FnData::graphListData() const
{

    return g_list;

}

QList<QString> FnData::stringListData() const
{

    return s_list;

}

/////////////////////////////////////////////////////////

QString FnData::graphOutput() const
{

    QString output;

    output += "( ";

    // add vertices
    foreach(QString vertex, g_value.vertexList()) {
        output += vertex + " ";
    }

    output += ": ";

    foreach(QString edge, g_value.edgeList()) {
        output += "(" + edge + "," + g_value.initialVertex(edge) + "," + g_value.terminalVertex(edge) + ") ";
    }

    output += ")";

    return output;

}

QString FnData::integerOutput() const
{


  return QString::number(i_value);

}

QString FnData::mapOutput() const
{

    int rank = f_value.imageRank();

    QString output;

    output += "{ ";

    for(int i = 0; i < 2*rank-2; i+=2) {
        output += f_value.value(BASIS.at(i)) + ", ";
    }

    output += f_value.value(BASIS.at(2*rank-2));
    output += " : " + QString::number(rank) + " }";

    return output;

}

QString FnData::graphListOutput() const
{

    QString output;

    output = "[ ";

    foreach(FnGraph Gamma, g_list) {
        output += FnData(Gamma).graphOutput() + ", ";
    }

    output.remove(output.length()-2,2); // remove final ", "
    output += " ]";

    return output;

}

QString FnData::stringListOutput() const
{

    QString output;

    output = "[ ";

    foreach(QString list_element, s_list) {
        output += list_element + ", ";
    }

    output.remove(output.length()-2,2); // remove final ", "
    output += " ]";

    return output;

}

QString FnData::wordOutput() const
{

    return wordData();

}

/////////////////////////////////////////////////////////

QString FnData::toOutput() const
{

    QString output;

    switch(data_type) {

    case Element:
        output = wordOutput();
        break;

    case Graph:
        output = graphOutput();
        break;

    case GraphList:
        output = graphListOutput();
        break;

    case Integer:
        output = integerOutput();
        break;

    case Morphism:
        output = mapOutput();
        break;

    case StringList:
        output = stringListOutput();
        break;

    case FailMessage:
        output = failMessage;
        break;

    default:
        output = failMessage;
        break;

    }

  return output;

}

/////////////////////////////////////////////////////////

void FnData::setInteger(int i)
{

  data_type = Integer;
  i_value = i;

}

void FnData::setElement(FnWord &u)
{

    data_type = Element;
    u_value = u;

}

void FnData::setGraph(FnGraph &Gamma)
{

    data_type = Graph;
    g_value = Gamma;

}

void FnData::setMorphism(FnMap &phi)
{

  data_type = Morphism;
  f_value = phi;

}

void FnData::setGraphList(QList<FnGraph> &list)
{

    data_type = GraphList;
    g_list = list;

}

void FnData::setStringList(QList<QString> &list)
{

    data_type = StringList;
    s_list = list;

}

void FnData::setFailMessage(QString fail)
{

  data_type = FailMessage;
  failMessage = fail;

}

/////////////////////////////////////////////////////////

