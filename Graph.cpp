//
/////////////////////////////////////////////////////////

#include "Graph.h"

Graph::Graph()
{

}

/////////////////////////////////////////////////////////

void Graph::addVertex(const QString &vertex, bool *ok) {

    if(!vertices.contains(vertex)) {
        vertices.append(vertex);
        *ok = true;
    }

}

void Graph::addEdge(const QString &edge, const QVector<QString> &endpoints, bool *ok) {

    if(!keys().contains(edge)) {
        insert(edge,endpoints);
        *ok = true;
    }

}

void Graph::addEdge(const QString &edge, const QString &initial, const QString &terminal, bool *ok) {

    QVector<QString> endpoints;
    endpoints << initial << terminal;
    addEdge(edge,endpoints,ok);

}

/////////////////////////////////////////////////////////
