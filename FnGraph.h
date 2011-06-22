// Header file for class FnGraph
// inherits QHash<QString, QVector<QString> >

// author Matt Clay
// version 110407

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

    QList<QString> vertexList() const { return vertices; }
    QList<QString> edgeList() const { return keys(); }

private:
    QList<QString> vertices;

};

typedef QHashIterator<QString, QVector<QString> > GraphIterator;
typedef QMutableHashIterator<QString, QVector<QString> > GraphMutableIterator;

#endif // FNGRAPH_H
