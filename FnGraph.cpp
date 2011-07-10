//
/////////////////////////////////////////////////////////

#include "FnGraph.h"

FnGraph::FnGraph()
{

}

/////////////////////////////////////////////////////////

void FnGraph::addVertex(const QString &vertex) {

    // adds the vertex named vertex with an empty list
    // of adjacent edges

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

void FnGraph::removeVertex(const QString &vertex)
{
    QVector<QString> edges=vertices.take(vertex);
    while(!edges.isEmpty())
    {
        removeEdge(edges.at(0));
        edges.pop_front();
    }
}

void FnGraph::removeEdge(const QString &edge)
{
    QVector<QString> nodes= take(edge);
    QVector<QString> edges;
    if(vertices.contains(nodes.at(0)))
    {
        edges=vertices.value(nodes.at(0));
        vertices[nodes.at(0)].remove(edges.indexOf(edge));
    }
    else
         vertices[nodes.at(1)].remove(vertices.value(nodes.at(1)).indexOf(edge));
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
    QHash<QString, QVector <QString> > unused=vertices;
    QList<QString> queue;
    QString edge,vertex;

    while(!unused.isEmpty())
    {
        FnGraph component;
        queue.append(unused.begin().key());//puts the first unused vertex it finds in the queue

        while(!queue.isEmpty())
        {
            vertex=queue[0];//sets the vertex to next key in the queue
            component.addVertex(vertex);
            foreach(edge,unused.value(vertex))
            {
                if(!component.contains(edge))//if the edge hasn't been used
                {
                    component.addEdge(edge,value(edge));//add it to the graph

                    if(initialVertex(edge)!=vertex)//finds the other vertex along the edge
                    {
                        if(!queue.contains(initialVertex(edge)))//if the other vertex isn't in the queue
                            queue.push_back(initialVertex(edge));//add it
                    }
                    if(terminalVertex(edge)!=vertex)
                    {
                        if(!queue.contains(terminalVertex(edge)))
                            queue.push_back(terminalVertex(edge));
                    }
                }

            }
            queue.pop_front();//removes the vertex from the queue
            unused.remove(vertex);//the vertex has been used
        }
        components.append(component);
    }


    return components;

}

QList<FnGraph> FnGraph::biconnectedComponents() const {

  QList<FnGraph> list;

  FnGraph Gamma1, Gamma2;

  Gamma1.addEdge(QString("e0"),QString("v0"),QString("v1"));
  Gamma2.addEdge(QString("e1"),QString("w0"),QString("w0"));

  list.append(Gamma1);
  list.append(Gamma2);

  return list;

}

//Friends
FnGraph operator + (const FnGraph & gamma, const FnGraph & beta)
{
    FnGraph tau;
    QString num;
    foreach(QString vertex, gamma.vertices.keys())
        tau.addVertex(vertex);
    foreach(QString vertex, beta.vertices.keys())
        tau.addVertex(vertex);

    foreach(QString edge, gamma.keys())
    {
        tau.addEdge("e."+num.setNum(tau.size()),gamma.value(edge));
    }
    foreach(QString edge, beta.keys())
    {
        tau.addEdge("e."+num.setNum(tau.size()),beta.value(edge));
    }

    return tau;
}


/////////////////////////////////////////////////////////
