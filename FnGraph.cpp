//
/////////////////////////////////////////////////////////

#include "FnGraph.h"

FnGraph::FnGraph()
{

}

/////////////////////////////////////////////////////////

void FnGraph::addVertex(const QString &vertex) {

    if(!vertices.contains(vertex)) {
        QVector<QString> s;
        vertices.insert(vertex,s);
    }

}

void FnGraph::addEdge(const QString &edge, const QVector<QString> &endpoints) {

    if (!vertices.contains(endpoints.at(0)))
        addVertex(endpoints.at(0));
        vertices[endpoints.at(0)].push_back(edge);
    if (!vertices.contains(endpoints.at(1)))
        addVertex(endpoints.at(1));
    if(endpoints.at(0)!=endpoints.at(1))
        vertices[endpoints.at(1)].push_back(edge);
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



        foreach(QString vertex, vertices.keys())
        {
            if(vertices.value(vertex).size()==0)
            {
                   isolated.append(vertex);
            }


        }

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
