// Header file for class FnGraph
// inherits QHash<QString, QVector<QString> >

// authors Matt Clay, Jack Conant and Nivetha Ramasubramanian
// version 120511

/////////////////////////////////////////////////////////

#ifndef FNGRAPH_H
#define FNGRAPH_H

#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVector>

#include "Fn.h"

/////////////////////////////////////////////////////////

class FnGraph : public QHash<QString, QVector<QString> >
{

public:
    FnGraph();

    int nVertices(void) const { return vertices.size(); }
    int nEdges(void) const { return size(); }
    int eulerCharacteristic(void) const { return (vertices.size() - size()); }
    QString initialVertex(const QString &edge) const {
        return value(edge).at(0);
    }
    QString terminalVertex(const QString &edge) const {
        return value(edge).at(1);
    }

    void addVertex(const QString &vertex);
    void addEdge(const QString &edge, const QVector<QString> &endpoints);
    void addEdge(const QString &edge, const QString &initial, const QString &terminal);

    void removeVertex(const QString &vertex);
    void removeEdge(const QString &edge);

    QStringList adjacentEdges(const QString &vertex) const {
        return vertices.value(vertex);
    }

    QVector<QString> adjacentVertices(const QString &edge) const {
        return value(edge);
    }

    QStringList isolatedVertices(void) const;
    QList<FnGraph> connectedComponents(void) const;
    QList<FnGraph> biconnectedComponents(void) const;

    QStringList vertexList(void) const { return vertices.keys(); }
    QStringList edgeList(void) const { return keys(); }

    bool isSubGraph(const FnGraph &Gamma) const;

    //friends
    friend FnGraph operator + (const FnGraph &Gamma0, const FnGraph &Gamma1);
    friend FnGraph operator - (const FnGraph &Gamma0, const FnGraph &Gamma1);

private:
    QHash<QString, QStringList> vertices; // holds list of vertices and their adjacent edges

};

typedef QHashIterator<QString, QVector<QString> > GraphIterator;
typedef QMutableHashIterator<QString, QVector<QString> > GraphMutableIterator;

#endif // FNGRAPH_H
