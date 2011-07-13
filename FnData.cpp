//
/////////////////////////////////////////////////////////

#include "Basis.h"
#include "FnData.h"

/////////////////////////////////////////////////////////

// CONSTRUCTORS
FnData::FnData()
{

  data_type = NoType;
  list = false;
  failMessage = "Unknown data type encountered for FnData.";

}

FnData::FnData(int i)
{

  data_type = Integer;
  list = false;
  i_list.prepend(i);

}

FnData::FnData(const FnGraph &Gamma)
{

    data_type = Graph;
    list = false;
    g_list.prepend(Gamma);

}

FnData::FnData(const FnMap &phi)
{

  data_type = Morphism;
  list = false;
  f_value = phi;

}

FnData::FnData(const FnWord &u)
{

  data_type = Element;
  list = false;
  u_list.prepend(u);

}

FnData::FnData(const QString &s)
{

  data_type = String;
  list = false;
  s_list.prepend(s);

}

FnData::FnData(const QList<int> &integerList)
{

    data_type = Integer;
    list = true;
    i_list = integerList;

}

FnData::FnData(const QList<FnGraph> &graphList)
{

    data_type = Graph;
    list = true;
    g_list = graphList;

}

FnData::FnData(const QList<FnWord> &wordList)
{

    data_type = Element;
    list = true;
    u_list = wordList;

}

FnData::FnData(const QList<QString> &stringList)
{

    data_type = String;
    list = true;
    s_list = stringList;

}

/////////////////////////////////////////////////////////

int FnData::integerData() const
{

    return i_list.at(0);

}

FnGraph FnData::graphData() const
{

    return g_list.at(0);

}

FnMap FnData::mapData() const
{

    return f_value;

}

QString FnData::stringData() const
{

    return s_list.at(0);

}

FnWord FnData::wordData() const
{

    return u_list.at(0);

}

QList<int> FnData::integerListData() const
{

    return i_list;

}

QList<FnGraph> FnData::graphListData() const
{

    return g_list;

}

QList<FnWord> FnData::wordListData() const
{

    return u_list;

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
    foreach(QString vertex, g_list.at(0).vertexList()) {
        output += vertex + " ";
    }

    output += ": ";

    foreach(QString edge, g_list.at(0).edgeList()) {
        output += "(" + edge + "," + g_list.at(0).initialVertex(edge) + "," + g_list.at(0).terminalVertex(edge) + ") ";
    }

    output += ")";

    return output;

}

QString FnData::integerOutput() const
{


  return QString::number(i_list.at(0));

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

QString FnData::stringOutput() const
{

    return stringData();

}

QString FnData::wordOutput() const
{

    return wordData();

}

QString FnData::integerListOutput() const
{

    QString output;

    output = "[ ";

    foreach(int i, i_list) {
        output += QString::number(i) + ", ";
    }

    output.remove(output.length()-2,2); // remove final ", "
    output += " ]";

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

QString FnData::wordListOutput() const
{

    QString output;

    output = "[ ";

    foreach(FnWord u, u_list) {
        output += u + ", ";
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

/////////////////////////////////////////////////////////

QString FnData::toOutput() const
{

    QString output;


    switch(data_type) {

        case Element:
            if(list)
                output = wordListOutput();
            else
                output = wordOutput();
            break;

        case Graph:
            if(list)
                output = graphListOutput();
            else
                output = graphOutput();
            break;

        case Integer:
            if(list)
                output = integerListOutput();
            else
                output = integerOutput();
            break;

        case Morphism:
            output = mapOutput();
            break;

        case String:
            if(list)
                output = stringListOutput();
            else
                output = stringOutput();
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
    list = false;
    i_list.clear();
    i_list.prepend(i);

}

void FnData::setElement(FnWord &u)
{

    data_type = Element;
    list = false;
    u_list.clear();
    u_list.prepend(u);

}

void FnData::setGraph(FnGraph &Gamma)
{

    data_type = Graph;
    list = false;
    g_list.clear();
    g_list.prepend(Gamma);

}

void FnData::setMorphism(FnMap &phi)
{

    data_type = Morphism;
    list = false;
    f_value = phi;

}

void FnData::setString(QString s)
{

    data_type = String;
    list = false;
    s_list.clear();
    s_list.prepend(s);

}

void FnData::setIntegerList(QList<int> &integerList)
{

    data_type = Integer;
    list = true;
    i_list = integerList;

}

void FnData::setGraphList(QList<FnGraph> &graphList)
{

    data_type = Graph;
    list = true;
    g_list = graphList;

}

void FnData::setWordList(QList<FnWord> &wordList)
{

    data_type = Element;
    list = true;
    u_list = wordList;

}

void FnData::setStringList(QList<QString> &stringList)
{

    data_type = String;
    list = true;
    s_list = stringList;

}

void FnData::setFailMessage(QString fail)
{

    data_type = FailMessage;
    list = false;
    failMessage = fail;

}

void FnData::addToList(const FnData &listItem)
{

    if(data_type != listItem.data_type) {
        setFailMessage(QObject::tr("Data Error: Inconsistent types encountered in list."));
        return;
    }

    if(listItem.isList()) {
        setFailMessage(QObject::tr("Data Error: Variable type cannot be listed."));
        return;
    }

    list = true;

    switch(data_type) {

        case Element:
            u_list.append(listItem.wordData());
            break;

        case Graph:
            g_list.append(listItem.graphData());
            break;

        case Integer:
            i_list.append(listItem.integerData());
            break;

        case String:
            s_list.append(listItem.stringData());
            break;

        default:
            setFailMessage(QObject::tr("Data Error: Variable type cannot be listed."));
            break;

    }

}

QList<FnData> FnData::listData() const
{

    FnData data;
    QList<FnData> dataList;

    if(!list) {
        data.setFailMessage(QObject::tr("Data Error: not a list."));
        dataList.append(data);
        return dataList;
    }

    switch(data_type) {

        case Element:
            foreach(FnWord u, u_list) {
                data.setElement(u);
                dataList.append(data);
            }
            break;

        case Graph:
            foreach(FnGraph Gamma, g_list) {
                data.setGraph(Gamma);
                dataList.append(data);
            }
            break;

        case Integer:
            foreach(int i, i_list) {
                data.setInteger(i);
                dataList.append(data);
            }
            break;

        case String:
            foreach(QString string, s_list) {
                data.setString(string);
                dataList.append(data);
            }
            break;

        default:
            data.setFailMessage(QObject::tr("Data Error: Variable type cannot be listed."));
            dataList.append(data);
            break;

    }

    return dataList;

}

/////////////////////////////////////////////////////////
// Other Functions

FnData loadGraphData(const QString &graphData)
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

FnData loadMorphismData(const QString &morphismData) {

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

