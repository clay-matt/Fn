//
/////////////////////////////////////////////////////////

#include "FnGraph.h"

FnGraph::FnGraph()
{

}

/////////////////////////////////////////////////////////

void FnGraph::addVertex(const QString &vertex)
{

    // adds the vertex named vertex with an empty list
    // of adjacent edges

    // if the vertex name is already used, there are no
    // changes to the graph

    if(!vertices.contains(vertex)) {

        QStringList emptyList;
        vertices.insert(vertex,emptyList);

    }

}

void FnGraph::addEdge(const QString &edge, const QVector<QString> &endpoints)
{

    // adds the edge named edge with adjacent vertices
    // given by endpoints

    // if the edge name is already used, there are no changes to the graph

    if(!keys().contains(edge)) {

        // if the vertices are not present, they are first
        // added to the graph

        if (!vertices.contains(endpoints.at(0)))
            addVertex(endpoints.at(0));
        vertices[endpoints.at(0)].append(edge);

        if (!vertices.contains(endpoints.at(1)))
            addVertex(endpoints.at(1));
        vertices[endpoints.at(1)].append(edge);

        insert(edge,endpoints);

    }

}

void FnGraph::addEdge(const QString &edge, const QString &initial, const QString &terminal)
{

    // overloads addEdge(QString, QVector<QString>) by creating a QVector
    // with entries initial and terminal

    QVector<QString> endpoints;
    endpoints << initial << terminal;
    addEdge(edge,endpoints);

}

void FnGraph::removeVertex(const QString &vertex)
{

    // removes vertex from the graph and any adjacent edges

    // if vertex is not present, there is no change in the graph

    QStringList adjacentEdgeList = vertices.take(vertex);

    foreach(QString edge, adjacentEdgeList) {
        removeEdge(edge);
    }

}

void FnGraph::removeEdge(const QString &edge)
{

    // removes edge from the graph

    // if edge is not present, there is no change in the graph

    if(contains(edge)) {

        QString v0 = initialVertex(edge);
        QString v1 = terminalVertex(edge);

        remove(edge);

        // remove edge from list of adjacent edges
        if(vertices.contains(v0))
            vertices[v0].removeAll(edge);

        if(v0 != v1 && vertices.contains(v1))
            vertices[v1].removeAll(edge);

    }

}

QStringList FnGraph::isolatedVertices(void) const
{

    // returns the list of isolated vertices of the graph

    QStringList isolated;

    foreach(QString vertex, vertices.keys()) {

        if(vertices.value(vertex).isEmpty())
            isolated.append(vertex);

        }

    return isolated;

}

QList<FnGraph> FnGraph::connectedComponents() const
{

    // returns the list of connected components of the graph

    QList<FnGraph> components;
    QHash<QString, QStringList> unusedVertices = vertices;
    QList<QString> queue;

    while(!unusedVertices.isEmpty()) {

        FnGraph component;

        queue.append(unusedVertices.keys().first()); // puts the first unused vertex it finds in the queue

        while(!queue.isEmpty()) {

            QString vertex = queue.at(0); // sets vertex to the next one in the queue

            component.addVertex(vertex); // add vertex to component

            // loop over edges adjacent to vertex
            foreach(QString edge, unusedVertices.value(vertex)) {

                if(!component.contains(edge)) { // if the edge hasn't been used

                    component.addEdge(edge,adjacentVertices(edge)); // add it to component

                    // add other adjacent vertex to queue
                    if(initialVertex(edge) != vertex) { // finds the other vertex along the edge

                        if(!queue.contains(initialVertex(edge))) // if the other vertex isn't in the queue
                            queue.append(initialVertex(edge)); // add it

                    }

                    if(terminalVertex(edge) != vertex) {

                        if(!queue.contains(terminalVertex(edge)))
                            queue.append(terminalVertex(edge));

                    }

                }

            }

            queue.removeFirst(); // removes vertex from the queue
            unusedVertices.remove(vertex); // removes vertex from vertex list

        }

        components.append(component); // add component to component list

    }

    return components;

}

QList<FnGraph> FnGraph::biconnectedComponents() const
{

    // returns the list of biconnected components of the graph

    /*
    This algorithm was derived from an algorithm found in John Hopcroft's and Robert Tarjan's article
    "Algorithm 447 Efficient Algorithms for Graph Manipulation" in Communications of the ACM, 1973
    */

    QList<FnGraph> bicomponents;
    FnGraph Gamma(*this);
    QList<QString> edgeStack;
    QList<QString> vertexStack;
    QList<int> dFSNumbers; // stores the Depth First Search number of each vertex;
    QList<int> lowpoints; // stores the lowpoint of each vertex;
    QString vertex;
    QString edge;
    int count = 1; // keeps track of the number vertices searched

    // remove isolated vertices from the graph
    foreach(QString vertex, isolatedVertices()) {
        Gamma.removeVertex(vertex);
    }

    // add first vertex to stack
    vertexStack.prepend(Gamma.vertexList().first());

    dFSNumbers.prepend(count); // sets the number of the vertex
    lowpoints.prepend(Gamma.nVertices()); // sets the lowpoint to the number of vertices as a place holder.

    // main loop
    while (vertexStack.size() > 1 || !Gamma.adjacentEdges(vertexStack.first()).isEmpty()) {


        // if there is an edge from the top of the vertex stack
        if (!Gamma.adjacentEdges(vertexStack.first()).isEmpty()) {

            edge = Gamma.adjacentEdges(vertexStack.first()).first();

            // set head to other adjacent vertex (if one exists)
            if (vertexStack.first() == Gamma.initialVertex(edge))
                vertex = Gamma.terminalVertex(edge);
            else
                vertex = Gamma.initialVertex(edge);

            if (vertexStack.contains(vertex)) {

                if(dFSNumbers.at(vertexStack.indexOf(vertex)) < lowpoints.first())
                    lowpoints[0] = dFSNumbers.at(vertexStack.indexOf(vertex));

            }

            else {

                count++;
                vertexStack.prepend(vertex);
                lowpoints.prepend(dFSNumbers.first());
                dFSNumbers.prepend(count);

            }

            // delete edge from Gamma and place at top of edge stack
            edgeStack.prepend(edge);
            Gamma.removeEdge(edge);

        } // now retest while statement

        // there are no edges out of the top of the stack, but there is more than one point in stack
        else {

            if (lowpoints.first() == dFSNumbers.at(1)) { // make new biconnected component

                FnGraph component;

                // if there are only two vertices, take all edges
                if (vertexStack.size() == 2) {

                    while(!edgeStack.isEmpty()) {

                        component.addEdge(edgeStack.first(),adjacentVertices(edgeStack.first()));
                        edgeStack.removeFirst();

                    }

                }

                // add edges from edge stck until one is found that connects to third point on stack
                else {

                    vertex = vertexStack.at(2);

                    while(!edgeStack.isEmpty() && !adjacentVertices(edgeStack.first()).contains(vertex)) {

                            component.addEdge(edgeStack.first(),adjacentVertices(edgeStack.first()));
                            edgeStack.removeFirst();

                    }

                }

                bicomponents.append(component);

          }

          else { // modify lowpoints if necessary

                if(lowpoints.first() < lowpoints.at(1))
                  lowpoints[1] = lowpoints.first();

          }

          // remove top of stack
          vertexStack.removeFirst();
          dFSNumbers.removeFirst();
          lowpoints.removeFirst();

     } // now retest while statement

  }

  // if Gamma is not connected, repeat process
  if(count < Gamma.nVertices())
      bicomponents += Gamma.biconnectedComponents();

  return bicomponents;

}

bool FnGraph::isSubGraph(const FnGraph &Gamma) const
{

    // determines if graph is a subgraph of Gamma

    // in order to be a subgraph, it must have the same vertex and edge labels
    // and adjacency

    // test that each vertex is also a vertex of Gamma
    foreach(QString vertex, vertexList()) {

        if(!Gamma.vertexList().contains((vertex)))
            return false;

    }

    // test that each edge is an edge of Gamma with same adjacent vertices
    foreach(QString edge, edgeList()) {

        if(!Gamma.edgeList().contains(edge))
            return false;

        if(adjacentVertices(edge) != Gamma.adjacentVertices(edge))
            return false;

    }

    return true;

}

/////////////////////////////////////////////////////////
// Friends

FnGraph operator + (const FnGraph &Gamma0, const FnGraph &Gamma1)
{

    // returns the sum the two graphs Gamma0 and Gamma1

    // the vertices of the sum is the union of the two vertex lists
    // the edges of the sum is the disjoint union of the two edge lists
    // if an edge has the same label in Gamma1, the label is appended with '

    FnGraph unionGraph;

    QString edge,vertex;

    foreach(vertex, Gamma0.vertexList())
        unionGraph.addVertex(vertex);

    foreach(vertex, Gamma1.vertexList())
        unionGraph.addVertex(vertex);

    foreach(edge, Gamma0.edgeList())
        unionGraph.addEdge(edge,Gamma0.adjacentVertices(edge));

    foreach(edge, Gamma1.edgeList()) {

        if(unionGraph.edgeList().contains(edge))
            unionGraph.addEdge(edge+"'",Gamma1.adjacentVertices(edge));

        else
            unionGraph.addEdge(edge,Gamma1.adjacentVertices(edge));

    }

    return unionGraph;

}

FnGraph operator - (const FnGraph &Gamma0, const FnGraph &Gamma1)
{

    // returns the difference of the graphs Gamma0 and Gamma1 if
    // Gamma1 is a subgraph of Gamma0, else returns Gamma0

    // this is the graph Gamma0 with all of the edges of Gamma1 removed
    // additionally any vertex only adjacent to edges in Gamma1 is removed
    // and any isolated vertex of Gamma1 that is isolated in Gamma0 is also removed

    FnGraph differenceGraph(Gamma0);

    if(Gamma1.isSubGraph(differenceGraph)) {

        // remove edges of Gamma1
        foreach(QString edge, Gamma1.edgeList())
            differenceGraph.removeEdge(edge);

        // remove certain isolated vertices
        QStringList isolated = differenceGraph.isolatedVertices();
        foreach(QString vertex, isolated) {

            if(Gamma1.vertexList().contains(vertex))
                differenceGraph.removeVertex(vertex);

        }

    }

    return differenceGraph;

}

/////////////////////////////////////////////////////////
