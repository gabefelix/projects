/*
 *Gabriel Velazquez
 * gvelazq3 / 1330468
 * ASG4
 */

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// Constructors & Destructors //////////////////////////////

//newGraph() returns a Graph pointing to a newly created GraphObj representing a
//graph having n vertices and no edges. Pre: n!=0 post: new graph with n nodes
Graph newGraph(int n) {
	Graph G = malloc(sizeof(struct GraphObj));

	G->adj = calloc(n+1, sizeof( List));
	G->color = calloc(n+1, sizeof(int));
	G->d = calloc(n+1, sizeof(int));
	G->P = calloc(n+1, sizeof(int));
	G->order = n;
	G->size = 0;
	G->source = NIL;

	for (int i = 1; i <= n; ++i) {
		G->adj[i] = newList();
		G->color[i] = WHITE;
		G->d[i] = INF;
		G->P[i] = NIL;
	}
	return (G);
}

// freeGraph() frees all dynamic memory associated with the Graph *pG, then sets the handle *pG to NULL.
void freeGraph(Graph* pG) {
	if (pG == NULL || (*pG) == NULL) {
		return;
	}

	for (int i = 1; i <= getOrder(*pG); ++i) {
		freeList(&((*pG)->adj[i]));
	}
	free((*pG)->adj);
	free((*pG)->color);
	free((*pG)->d);
	free((*pG)->P);
	free(*pG);
	*pG = NULL;
}

// Access Functions //////////////////////////////

// getOrder() returns the corresponding field values
int getOrder(Graph G) {
	if (G == NULL) {
		printf("Graph Error: calling getOrder() on NULL Graph\n");
		exit(1);
	}
	return G->order;
}

// getSize() returns the corresponding field values
int getSize(Graph G) {
	if (G == NULL) {
		printf("Graph Error: calling getSize() on NULL Graph\n");
		exit(1);
	}
	return G->size;
}

// getSource() returns the source vertex most recently used in function
int getSource(Graph G) {
	if (G == NULL) {
		printf("Graph Error: calling getSource() on NULL Graph\n");
		exit(1);
	}
	return G->source;
}

// getParent() return the parent of vertex u.
// Pre: 1<= u <= getOrder(G)
int getParent(Graph G, int u) {
	if (G == NULL) {
		printf("Graph Error: calling getParent() on NULL Graph\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		printf("Graph Error: calling getParent() on out of index array P\n");
		exit(1);
	}
	return G->P[u];
}

// getDist() returns the distance from the most recent BFS source to vertex
//u, or INF if BFS() has not yet been called.
//Pre: 1<= u <= getOrder(G)

int getDist(Graph G, int u) {
	if (G == NULL) {
		printf("Graph Error: calling getDist() on NULL Graph\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		printf("Graph Error: calling getDist() on out of index array d\n");
		exit(1);
	}
	return G->d[u];
}

// getPath() appends to the List L the vertices of a shortest path in G from source to u, or appends to L the value NIL if no such path exists.
// Pre: getSource(G) !=NIL and 1<= u <= getOrder(G)
void getPath(List L, Graph G, int u) {
	if (G == NULL) {
		printf("Graph Error: calling getPath() on NULL Graph\n");
		exit(1);
	}
	if (L == NULL) {
		printf("Graph Error: calling getPath() on NULL List\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		printf("Graph Error: calling getPath() on out of index array d\n");
		exit(1);
	}
	if (getSource(G) == NIL) {
		printf("Graph Error: calling getpath() on NIL source\n");
		exit(1);
	}

	if (u == getSource(G)) {
		append(L, u);
	} else if (getParent(G, u) == NIL) {
		append(L, NIL);
	} else {
		prepend(L, u);
		int x = getParent(G, u);
		while ( x != getSource(G) && x != NIL ) {
			prepend(L, x);
			x = getParent(G, x);
		}
		if ( x == NIL ) {
			prepend(L, NIL);
		} else {
			prepend(L, x);
		}
	}
}

// Manipulation Procedures //////////////////////////////

// makeNull() deletes all edges of G, restoring it to its original (no edge) state.
void makeNull(Graph G) {
	if (G == NULL) {
		printf("Graph Error: calling makeNull() on NULL Graph\n");
		exit(1);
	}

	for (int i = 1; i <= getOrder(G); ++i) {
		clear(G->adj[i]);
		G->color[i] = NIL;
		G->d[i] = NIL;
		G->P[i] = NIL;
	}
	G->size = 0;
	G->source = NIL;
}

// addEdge() inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, and v to the adjacency List of u.
// Pre: two int arguments must lie in the range 1 to getOrder(G)
void addEdge(Graph G, int u, int v) {
	if (G == NULL) {
		printf("Graph Error: calling addEdge() on NULL Graph\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		printf("Graph Error: calling addEdge() on u non-existent within bounds of order\n");
		exit(1);
	}
	if (v < 1 || v > getOrder(G)) {
		printf("Graph Error: calling addEdge() on v non-existent within bounds of order\n");
		exit(1);
	}

	List  listp;
	int inserted=0;
	listp = G->adj[u];
	if (length(listp) <=0)
		append(G->adj[u], v);

	else {
		moveTo(listp, 0);
		while (!inserted && getIndex(listp) < length(listp)-1)
		{
			if (getElement(listp) < v)
				moveNext(listp);
			else {
				insertBefore(listp, v);
				inserted = 1;
			}
		}
		if (!inserted)
			insertAfter(listp, v);
	}
	inserted = 0;
	listp = G->adj[v];
	if (length(listp) <=0)
		append(G->adj[v], u);
	else {
		moveTo(listp, 0);
		while (!inserted && getIndex(listp) < length(listp)-1)
		{
			if (getElement(listp) < u)
				moveNext(listp);
			else {
				insertBefore(listp, u);
				inserted = 1;
			}
		}
		if (!inserted)
			insertAfter(listp, u);
	}
	++G->size;
}

// addArc()  inserts a new directed edge from u to v.
// Pre: two int arguments must lie in the range 1 to getOrder(G)
void addArc(Graph G, int u, int v) {
	if (G == NULL) {
		printf("Graph Error: calling addArc() on NULL Graph\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		printf("Graph Error: calling addArc() on u non-existent within bounds of order\n");
		exit(1);
	}
	if (v < 1 || v > getOrder(G)) {
		printf("Graph Error: calling addArc() on v non-existent within bounds of order\n");
		exit(1);
	}

	append(G->adj[u], v);
	++G->size;
}

// BFS() breadth first search algorithm on graph G, with source s
//runs the BFS algorithm on the Graph G with source s, setting the color, distance, parent, and source fields of G accordingly
void BFS(Graph G, int s) {
	if (G == NULL) {
		printf("Graph Error: calling BFS() on NULL Graph\n");
		exit(1);
	}
	if (s < 1 || s > getOrder(G)) {
		printf("Graph Error: calling BFS() on s non-existent within bounds of order\n");
		exit(1);
	}

	 G->source = s;
	int i, x, y;
	for (i = 1; i <= getOrder(G); ++i) {
		G->color[i] = WHITE;
		G->d[i] = INF;
		G->P[i] = NIL;
	}
	G->color[s] = GREY;
	G->d[s] = 0;
	G->P[s] = NIL;
	List L = newList();
	append(L, s);

	while (length(L)) {
		x = front(L);
		deleteFront(L);
		if (length(G->adj[x]) )
		{
			moveTo(G->adj[x], 0);
		}
		while (getIndex(G->adj[x]) >= 0) {
			y = getElement(G->adj[x]);
			if (G->color[y] == WHITE) {
				G->color[y] = GREY;
				G->d[y] = (getDist(G, x) + 1);
				G->P[y] = x;
				append(L, y);
				moveTo(L, 0);
				for (int i=0; i < length(L); i++)
				{
				   moveNext(L);
				}
			}
			else
				moveNext(G->adj[x]);
		}
		G->color[x] = BLACK;
	}
	freeList(&L);
}

// Other Operations //////////////////////////////

// printGraph() prints the adjacency list representation of G to the file pointed to by out.
void printGraph(FILE* out, Graph G){
	if (G == NULL) {
		printf("Graph Error: calling printGraph() on NULL Graph\n");
		exit(1);
	}

	int temp;

	for (int i = 1; i <= getOrder(G); i++) {
		fprintf(out, "%d: ", i);

		moveTo (G->adj[i], 0);
		if (length(G->adj[i])) {
			while ( getIndex(G->adj[i]) >= 0) {
				temp = getElement(G->adj[i]);
				moveNext(G->adj[i]);
				fprintf(out, " %d", temp);
			}
		}
		fprintf(out, "\n");
	}
}
