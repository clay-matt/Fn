// Header file for class Graph
// inherits QHash<QString, QVector<QString> >

// author Matt Clay
// version 110407

/////////////////////////////////////////////////////////

#ifndef GRAPH_H
#define GRAPH_H

#include <QHash>
#include <QList>
#include <QString>
#include <QVector>

#include "Fn.h"

/////////////////////////////////////////////////////////

class Graph : public QHash<QString, QVector<QString> >
{

public:
    Graph();

    int nVertices(void) const { return vertices.size(); }
    int nEdges(void) const { return size(); }
    int eulerCharacteristic(void) const { return (vertices.size() - size()); }
    QString initialVertex(const QString &edge) const {
        return value(edge).at(0);
    }
    QString terminalVertex(const QString &edge) const {
        return value(edge).at(1);
    }

    void addVertex(const QString &vertex, bool *ok = false);
    void addEdge(const QString &edge, const QVector<QString> &vertices, bool *ok = false);
    void addEdge(const QString &edge, const QString &initial, const QString &terminal, bool *ok = false);

//    QList<QString> vertexList() const { return vertices; }
//    QList<QString> edgeList() const { return keys(); }

private:
    QList<QString> vertices;

};

#endif // GRAPH_H
