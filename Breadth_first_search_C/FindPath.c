/*
 *Gabriel Velazquez
 * gvelazq3 / 1330468
 * ASG4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#define MAX_LEN 250

typedef enum {FALSE = 0, TRUE = 1} bool;

//prints out the destination and the path
void printDP(FILE* out, Graph G, List path, int dest) {
	fprintf(out, "Distance from %d to %d is ", getSource(G), dest);
	if (front(path) == NIL) {
		fprintf(out, "infinity\n");
		fprintf(out, "No %d-%d path exists", getSource(G), dest);
	} else {
		fprintf(out, "%d\n", getDist(G, dest));
		fprintf(out, "Shortest %d-%d path is: ", getSource(G), dest);
		printList(out, path);
	}
	fprintf(out,"\n");
}

int main (int argc, char * argv[]) {
	if (argc != 3) { // check for 2 args in command line
		printf("Usage: %s infile outfile\n", argv[0]);
		exit(1);
	}

	// open up file for writing
	FILE *in, *out;
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	if (in==NULL) {
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	if (out==NULL) {
		printf("Unable to open file %s for writing\n", argv[2]);
		exit(1);
	}

	// declare vars
	char line[MAX_LEN];
	char* token;
	int lineNumber = 0;
	bool flag = FALSE;
	int  n, x, y, source, dest;
	Graph G;
	List path = newList();

	// read and store graph, print out adj list
	while( !flag && fgets(line, MAX_LEN, in) != NULL ) {
		++lineNumber;
		token = strtok(line, " \n");
		if (lineNumber == 1) {
			n = atoi(token);
			G = newGraph(n);
		} else {
			x = atoi(token);
			token = strtok(NULL, " \n");
			y = atoi(token);
			if (x != 0) {
				addEdge(G, x, y);
			}
			else {
				flag = TRUE;
			}
		}
	}
	printGraph(out, G);
	fprintf(out, "\n");

	// read in pair of vertices, run BFS, print distance to destination vertex,
	// find and print shortest path
	while( fgets(line, MAX_LEN, in) != NULL ) {

		token = strtok(line, " \n");
		source = atoi(token);
		token = strtok(NULL, " \n");
		dest = atoi(token);
		if (source != NIL){
			BFS(G, source);
			clear(path);
			getPath(path, G, dest);
			printDP(out, G, path, dest);
		}
	}
	/* free memory */
	freeList(&path);
	freeGraph(&G);

	/* close files */
	fclose(in);
	fclose(out);

	return(0);
}



