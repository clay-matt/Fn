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
    QHash<QString, QVector <QString> > unused=vertices;
    QList<QString> queue;
    QString edge,vertex;
    int j=0;

    while(!unused.isEmpty())
    {
        FnGraph component;
        queue.append(unused.begin().key());//puts the first unused vertex it finds in the queue

        while(!queue.isEmpty())
        {
            vertex=queue[0];//sets the vertex to next key in the queue
            component.addVertex(vertex);
            for(j=0; j<unused.value(vertex).size();j++)//loops through all of the edges of vertex
            {

                edge=unused.value(vertex).at(j);
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

/////////////////////////////////////////////////////////
