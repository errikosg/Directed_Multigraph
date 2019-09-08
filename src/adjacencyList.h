#ifndef EDGE_H
#define EDGE_H

#include "stack.h"

//vertex represantation + adjacency list interface

typedef struct vertex* lnk;

typedef struct{
	lnk head;
	lnk tail;
	int count;						// = outDegree of this node / must be increased when vertex is added on this list or in each same list!
}adj_list;

struct vertex{
	int weight;						//weight of incoming edge
	char *name;						//name - varchar
	lnk next;						//link to next vertex in list - not same!
	adj_list same;					//list of same vertices as this one.
};


//functions
void adj_initialize(adj_list *);
int adj_isEmpty(adj_list *);
void adj_insert(adj_list *, char *, int);			//insert new vertex in adjacency list
void adj_insertSame(adj_list *, char *, int);		//insert new node in same list of specified vertex
void adj_delete(adj_list *, char *);				//delete every instance of "name" vertex from adjacency list.
void adj_deleteHead(adj_list *);
int adj_removeEdge(adj_list *, char *, int, int);	//removes an edge between current vertex and given / if many, remove head from same list
int adj_modifyWeight(adj_list *, char *, int, int);	//modifies given weight if exists.
int adj_printEdges(adj_list *, char *, char *);
lnk adj_find(adj_list *, char *name, lnk *);				//find and return vertex
int adj_search(adj_list *, char *);							//search if vertex exists in list
lnk adj_findWeight(adj_list *, int);
int adj_searchWeight(adj_list *, int);
int adj_pushAll(adj_list *, Stack *, Stack *, int, char *);
int adj_pushAll_v2(adj_list *, Stack *, Stack *, int, char *, int);
void adj_print(adj_list *);
void adj_print2(adj_list *);								//print only weights
void adj_WIF(adj_list *, FILE *);							//adj_WriteInFile(...)
void adj_freelist(adj_list *);	


#endif
