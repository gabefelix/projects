/*
 *Gabriel Velazquez
 * gvelazq3 / 1330468
 * ASG4
 */

#if !defined(_GRAPH_H_INCLUDE_)
#define _GRAPH_H_INCLUDE_
#define INF -1
#define NIL 0
#define WHITE 0
#define GREY 1
#define BLACK 2
#include "List.h"

typedef struct GraphObj {

    List* adj; //array of vertice numbers
    int *color; //color
    int *P; //array of parents
    int *d; //array of distances
    int order; //number of vertices
    int size; //number of edges
    int source; //the most recent vertex  source s
} GraphObj;

typedef struct GraphObj* Graph;

// Constructors & Destructors //////////////////////////////
Graph newGraph(int n); //returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges. Pre: n!=0 post: new graph with n nodes
void freeGraph(Graph* pG); //frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL.

// Access Functions //////////////////////////////
int getOrder(Graph G); //returns the corresponding field values
int getSize(Graph G); //returns the corresponding field values
int getSource(Graph G); //returns the source vertex most recently used in function
int getParent(Graph G, int u); //return the parent of vertex u. Pre: 1<= u <= getOrder(G)
int getDist(Graph G, int u); //returns the distance from the most recent BFS source to vertex u, or INF if BFS() has not yet been called.  Pre: 1<= u <= getOrder(G)
void getPath(List L, Graph G, int u); //appends to the List L the vertices of a shortest path in G from source to u, or appends to L the value NIL if no such path exists. Pre: getSource(G) !=NIL and 1<= u <= getOrder(G)


// Manipulation Procedures //////////////////////////////
void makeNull(Graph G); //deletes all edges of G, restoring it to its original (no edge) state.
void addEdge(Graph G, int u, int v); // inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, and v to the adjacency List of u. Pre: two int arguments must lie in the range 1 to getOrder(G)
void addArc(Graph G, int u, int v); // inserts a new directed edge from u to v. Pre: two int arguments must lie in the range 1 to getOrder(G)
void BFS(Graph G, int s); //runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly

// Other Operations //////////////////////////////
void printGraph(FILE* out, Graph G); //prints the adjacency list representation of G to the file pointed to by out.

#endif
