#ifndef GRAPH_H
#define GRAPH_H

#include "adjacencyList.h"
#include "stack.h"

//Simple static hash table(hashTable) with overflow chains(ovf_list).

typedef struct ovf_listNode* lk;
//list
struct ovf_listNode{
	char *name;
	adj_list al;
	lk next;
};

typedef struct{
	lk head;
	lk tail;
	int count;
}ovf_list;

/*-------------------------------*/

typedef struct{
	ovf_list ol;
	//char *name;
}ht_node;

typedef struct{
	ht_node **items;
	int count;
	int size;
}hashTable;

/*-----------------------------------------------------*/

typedef struct graph{
	int count;
	hashTable *ht;
}Graph;


//functions - hashTable
//static ht_node* ht_NewItem(char *);
void ht_initialize(hashTable **, int);					//initialize hash table with given size.
int ht_isEmpty(hashTable *);
void ht_insert(hashTable *, char *);	
void ht_insert_v2(hashTable *, char *, lk *);			//insert value and assign lk node 
int ht_delete(hashTable *, char *);	
void ht_clearInstances(hashTable *, char *);			//delete a node and all its instances in the graph (from all adjacency lists)
int ht_search(hashTable *, char *);
lk ht_find(hashTable *, char *);						//like search function but returns actual node
void ht_print(hashTable *);
int ht_showReceiving(hashTable *, char *);				//shows receiving edges
int ht_circleFind(hashTable *, char *);					//
int ht_findCircles(hashTable *, char *, int);
int ht_getTraceflow(hashTable *, char *, char *, int);
void ht_freeAll(hashTable *);
void ht_WIF(hashTable *, FILE *);
static int getHash(char *, int);
unsigned long hashfunction(char *);

//list functions
void ovf_initialize(ovf_list *);
int ovf_isEmpty(ovf_list *);
void ovf_insert(ovf_list *, char *);
void ovf_insert_v2(ovf_list *, char *, lk *);
int ovf_delete(ovf_list *, char *);
void ovf_clearInstance(ovf_list *, char *);
int ovf_showReceiving(ovf_list *, char *, int *);
int ovf_search(ovf_list *, char *);
lk ovf_find(ovf_list *, char *);
void ovf_print(ovf_list *);
void ovf_WIF(ovf_list *, FILE *);
void ovf_freeList(ovf_list *);

//Graph
void graph_initialize(Graph *, int);
int graph_isEmpty(Graph *);
lk graph_findVertex(Graph *, char *);						//find node and return it
int graph_addVertex_Sr(Graph *, char *);					//add new vertex in container list, without any edges yet - with searching!
lk graph_addVertex(Graph *, char *);						//adds vertex without searching first, and returns vertex inserted
void graph_addEdge(Graph*, char *, char *, int);			//add an edge from a vertex of container list = insert in adjacency list
int graph_deleteVertex(Graph *, char *);					//delete a vertex from container list / and from everywhere
int graph_deleteEdge(Graph *, char *,char *, int, int);		//delete an edge between "name", "to".
int graph_modifyWeight(Graph *, char *, char *, int, int);	//modifies weight between given nodes
int graph_showReceiving(Graph *, char *);					//shows receiving edges - searches whole graph
int graph_circleFind(Graph *, char *);						//find all simple circles
int graph_findCircles(Graph *, char *, int);				//find all closed walks
int graph_getTraceflow(Graph *, char *, char *, int);		//(start) - (end) - (max path length)
int graph_WIF(Graph *, char *);								//graph_WriteInFile(...)
void graph_print(Graph *);									//print container list with its adjacency lists.
void graph_freeAll(Graph *);
//and more


#endif
