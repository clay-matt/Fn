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
    if(nodes.at(0)==nodes.at(1))
    {
        if(vertices.contains(nodes.at(0)))
        {
            edges=vertices.value(nodes.at(0));
            vertices[nodes.at(0)].remove(edges.indexOf(edge));
        }
        return;
    }
    if(vertices.contains(nodes.at(0)))
    {
        edges=vertices.value(nodes.at(0));
        vertices[nodes.at(0)].remove(edges.indexOf(edge));
    }
     if(vertices.contains(nodes.at(1)))
    {
        edges=vertices.value(nodes.at(1));
         vertices[nodes.at(1)].remove(edges.indexOf(edge));
    }

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
    /*
    This algorithm was derived from an algorithm found in
    John Hopcroft's and Robert Tarjan's
    article "Algorithm 447 Efficient Algorithms for Graph Manipulation"
    in Communications of the ACM, 1973
    */
    //Note: I use the front of the stacks as the top of the stacks;
  QList<FnGraph> biconnectedComponents;
  FnGraph copyOfGraph= *this;
  QList<QString> edgeStack;
  QList<QString> vertexStack;
  QList<int> dFSNumbers;//stores the number of each vertex;
  QList<int> lowpoints;// stores the lowpoint of each vertex;
  QString head;//the other vertex along an edge
  QString edge;//the current edge being looked at
  int count=0;//keeps track of the number vertices searched

  vertexStack=copyOfGraph.isolatedVertices();
  while(!vertexStack.isEmpty())//removes isolated vertices from the graph
  {
      copyOfGraph.removeVertex(vertexStack[0]);
      vertexStack.pop_front();
  }
  vertexStack.push_front(copyOfGraph.vertexList().at(0));
  count++;
  dFSNumbers.push_front(count);//sets the number of the vertex
  lowpoints.push_front(copyOfGraph.vertices.size());//sets the Lowpoint to the number of vertices as a place holder.

  //chooses start point and puts it on the top of the stack;

  while((vertexStack.size()>1)||(copyOfGraph.vertices.value(vertexStack[0]).size()>0))
  {
      if(copyOfGraph.vertices.value(vertexStack[0]).size()>0)//if there is an edge from the top of the stack
      {
          edge=copyOfGraph.vertices.value(vertexStack[0]).at(0);

          if(copyOfGraph.value(edge).at(0)==vertexStack[0])
              head=copyOfGraph.value(edge).at(1);
          else
              head=copyOfGraph.value(edge).at(0);

          edgeStack.push_front(edge);
          copyOfGraph.removeEdge(edge);

          if(!vertexStack.contains(head))
          {
              vertexStack.push_front(head);//adds the new vertex to the stack
              count++;
              dFSNumbers.push_front(count);//numbers the vertex
              lowpoints.push_front(dFSNumbers[1]);// sets the lowpoint to the number of the previous top of stack

          }
          else
          {
              int stackPosition=vertexStack.indexOf(head);
              if(dFSNumbers[stackPosition]<lowpoints[0])
                  lowpoints[0]=dFSNumbers[stackPosition];
          }
      }
      else
      {
          if(lowpoints[0]==dFSNumbers[1])
          {
              FnGraph component;
              if(vertexStack.size()>2)
              {
                    while(!value(edgeStack[0]).contains(vertexStack[2]))
                    {
                        component.addEdge(edgeStack[0],value(edgeStack[0]));
                        edgeStack.pop_front();
                    }
              }
              else
              {
                  while(!edgeStack.isEmpty())
                  {
                      component.addEdge(edgeStack[0],value(edgeStack[0]));
                      edgeStack.pop_front();
                  }
              }
              biconnectedComponents.push_back(component);
          }
          else
          {
              if(lowpoints[0]<lowpoints[1])
                  lowpoints[1]=lowpoints[0];
          }
          vertexStack.pop_front();
          dFSNumbers.pop_front();
          lowpoints.pop_front();

     }
  }
  if(count<copyOfGraph.vertices.size())
  {
      biconnectedComponents+=copyOfGraph.biconnectedComponents();
  }



  return biconnectedComponents;

}

bool FnGraph::isSubGraph(const FnGraph &gamma) const
{
    foreach(QString vertex, vertices.keys())
    {
        if(!gamma.vertices.contains(vertex))
        {
            return false;
        }
    }

    foreach(QString edge, keys())
    {
        if(!gamma.contains(edge))
        {
            return false;
        }
    }
    return true;

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
FnGraph operator - (const FnGraph & gamma, const FnGraph & beta)
{
    FnGraph tau=gamma;

    if(beta.isSubGraph(gamma))
    {
        foreach(QString vertex, beta.vertices.keys())
        {
          tau.removeVertex(vertex);
        }
    }

    return tau;
}
/////////////////////////////////////////////////////////
