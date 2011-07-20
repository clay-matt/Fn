// Header file for class FnGraph
// inherits QHash<QString, QVector<QString> >

// authors Matt Clay, Jack Conant and Nivetha Ramasubramanian
// version 110627

/////////////////////////////////////////////////////////

#ifndef FNGRAPH_H
#define FNGRAPH_H

#include <QHash>
#include <QList>
#include <QString>
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
    void addEdge(const QString &edge, const QVector<QString> &vertices);
    void addEdge(const QString &edge, const QString &initial, const QString &terminal);

    void removeVertex(const QString &vertex);
    void removeEdge(const QString &edge);

    QVector<QString> getAdjacentEdges(const QString &vertex) const {
        return vertices.value(vertex);
    }

    QList<QString> isolatedVertices(void) const;
    QList<FnGraph> connectedComponents(void) const;
    QList<FnGraph> biconnectedComponents(void) const;

    QList<QString> vertexList() const { return vertices.keys(); }
    QList<QString> edgeList() const { return keys(); }

    void simplify();

    bool isSubGraph(const FnGraph &gamma) const;

    //friends
    friend FnGraph operator + (const FnGraph & gamma, const FnGraph & beta);
    friend FnGraph operator - (const FnGraph & gamma, const FnGraph & beta);
private:
    QHash<QString, QVector<QString> > vertices;

};

typedef QHashIterator<QString, QVector<QString> > GraphIterator;
typedef QMutableHashIterator<QString, QVector<QString> > GraphMutableIterator;

#endif // FNGRAPH_H
