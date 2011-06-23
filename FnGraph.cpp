//
/////////////////////////////////////////////////////////

#include "FnGraph.h"

FnGraph::FnGraph()
{

}

/////////////////////////////////////////////////////////

void FnGraph::addVertex(const QString &vertex) {

    if(!vertices.contains(vertex)) {
        vertices.append(vertex);
    }

}

void FnGraph::addEdge(const QString &edge, const QVector<QString> &endpoints) {

    if (!vertices.contains(endpoints.at(0)))
        addVertex(endpoints.at(0));

    if (!vertices.contains(endpoints.at(1)))
        addVertex(endpoints.at(1));

    if(!keys().contains(edge)) {
        insert(edge,endpoints);
    }

}

void FnGraph::addEdge(const QString &edge, const QString &initial, const QString &terminal) {

    QVector<QString> endpoints;
    endpoints << initial << terminal;
    addEdge(edge,endpoints);

}

QList<QString> FnGraph::isolatedVertices() const {

    // Returns the list of isolated vertices of the graph

    QList<QString> isolated;
    isolated << QString("test1") << QString("test2");

    return isolated;

}

QList<FnGraph> FnGraph::connectedComponents() const {

    // Return the list of connected components of the graph

    QList<FnGraph> components;

    FnGraph gamma1,gamma2;

    gamma1.addEdge(QString("e"),QString("v0"),QString("v1"));
    gamma2.addEdge(QString("f"),QString("w0"),QString("w1"));

    components << gamma1 << gamma2;

    return components;

}

/////////////////////////////////////////////////////////
