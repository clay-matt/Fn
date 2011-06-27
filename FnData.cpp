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
// Other Functions

FnData loadGraphData(const QString & graphData)
{

    FnData graph;
    FnGraph Gamma;

    QString input = graphData.simplified();
    // remove ( and )
    input.chop(1);
    input.remove(0,1);

    // extract vertex lists
    if(input.count(":") != 1) {
        graph.setFailMessage(QObject::tr("Invalid Input: %1 is not a graph").arg(graphData));
        return(graph);
    }
    QStringList vertices_edges = input.split(":",QString::SkipEmptyParts);
    QStringList vertexList = vertices_edges.at(0).split(QRegExp("\\s+"),QString::SkipEmptyParts);

    if(vertexList.isEmpty()) {
        graph.setFailMessage(QObject::tr("Invalid input: a graph needs vertices."));
        return(graph);
    }

    // add vertices
    foreach(QString vertex, vertexList) {
        if(Gamma.vertexList().contains(vertex)) {
            graph.setFailMessage(QObject::tr("Invalid Input: the vertex %1 appears multiple times.")
                                 .arg(vertex));
            return(graph);
        }
        Gamma.addVertex(vertex);
    }

    // add edges if necessary
    if(vertices_edges.size() == 2) {
        QString edges = vertices_edges.at(1).trimmed();
        edges.chop(1);
        edges.remove(0,1);
        QStringList edgeList = edges.split(QRegExp("\\)\\s*\\("),QString::SkipEmptyParts); // splits at ) possible whitespace (
        QStringList edgeData;
        QString edgeName;
        QVector<QString> edgeVertices(2);
        foreach(QString edge, edgeList) {
            edgeData = edge.split(",");
            if(edgeData.size() != 3) {
                graph.setFailMessage(QObject::tr("Invalid Input: ( %1 ) is not a valid edge.").arg(edge));
                return(graph);
            }
            edgeName = edgeData.at(0).trimmed();
            if(Gamma.contains(edgeName)) {
                graph.setFailMessage(QObject::tr("Invalid Input: the edge %1 has already been defined.")
                                     .arg(edgeName));
                return(graph);
            }
            // check that vertices are in the graph
            edgeVertices[0] = edgeData.at(1).trimmed();
            if(!Gamma.vertexList().contains(edgeVertices.at(0))) {
                graph.setFailMessage(QObject::tr("Invalid Input: %1 is not a vertex of %2.")
                                     .arg(edgeVertices.at(0)).arg(graphData));
                return(graph);
            }
            edgeVertices[1] = edgeData.at(2).trimmed();
            if(!Gamma.vertexList().contains(edgeVertices.at(1))) {
                graph.setFailMessage(QObject::tr("Invalid Input: %1 is not a vertex of %2.")
                                     .arg(edgeVertices.at(1)).arg(graphData));
                return(graph);
            }
            Gamma.addEdge(edgeName,edgeVertices);
        }
    }

    graph.setGraph(Gamma);

    return(graph);

}

FnData loadMorphismData(const QString & morphismData) {

    FnData morphism;

    QString images = morphismData;
    images.remove(QRegExp("\\s+")); // remove whitespace
    images.chop(1); // remove trailing }
    images.remove(0,1); // remove initial {

    // get images
    QStringList imageList = images.split(",",QString::SkipEmptyParts);

    int rank = imageList.size(); // try to determine image rank
    QString lastEntry = imageList.takeLast();
    if (lastEntry.contains(QChar(':'))) {
      int index = lastEntry.indexOf(QChar(':'));
      rank = lastEntry.mid(index+1).trimmed().toInt();
      if (rank < Fn_MinRank || rank > Fn_MaxRank) {
          morphism.setFailMessage(QObject::tr("Input Error: %1 is not a valid rank (%2 < rank < %3)")
                                      .arg(QString::number(rank)).arg(Fn_MinRank).arg(Fn_MaxRank));
          return(morphism);
      }
      lastEntry = lastEntry.left(index).trimmed();
    }
    imageList.append(lastEntry);

    FnMap phi(imageList,rank);
    if (!phi) {
        morphism.setFailMessage(QObject::tr("Input Error: images %1 are not in specified basis").arg(imageList.join(",")));
        return(morphism);
    }

    morphism.setMorphism(phi);

    return(morphism);

}

