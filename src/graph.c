#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"

static ht_node* ht_NewItem(char *name)
{
	ht_node *new = malloc(sizeof(ht_node));
	ovf_initialize(&new->ol);
	//new->name = strdup(name);
}

void ht_initialize(hashTable **ht, int size)
{
	(*ht) = malloc(sizeof(hashTable));
	(*ht)->items = calloc(size, sizeof(ht_node *));
	(*ht)->count = 0;
	(*ht)->size = size;

	int i;
	for(i=0; i<size; i++){
		(*ht)->items[i] = NULL;
	}
}

int ht_isEmpty(hashTable *ht)
{
	return (ht->count==0);
}

unsigned long hashfunction(char* str){			//algorithm djb2 from:	www.cse.yorku.ca/~oz/hash.html
	unsigned long hash = 5381;
	int c;
	while(c = *str++)
		hash = ((hash<<5)+hash)+c;	
	return hash;
}

static int getHash(char *str, int bucket_num)
{
	int value = (int)hashfunction(str) % bucket_num;
	return value;
}

void ht_insert(hashTable *ht, char *name)
{
	int i=0, index;
	index = getHash(name, ht->size);	index = abs(index);

	if(ht->items[index] == NULL){
		ht->items[index] = malloc(sizeof(ht_node));
		ovf_initialize(&(ht->items[index]->ol));
	}
	ovf_insert(&(ht->items[index]->ol), name);

	ht->count++;
}

void ht_insert_v2(hashTable *ht, char *name, lk *x)
{
	//ht_node *itm = ht_NewItem(name);
	int i=0, index;
	index = getHash(name, ht->size);	index = abs(index);

	if(ht->items[index] == NULL){
		ht->items[index] = malloc(sizeof(ht_node));
		ovf_initialize(&(ht->items[index]->ol));
		//ht->items[index] = itm;
	}
	lk node;
	ovf_insert_v2(&(ht->items[index]->ol), name, &node);
	*x = node;

	ht->count++;
}

int ht_delete(hashTable *ht, char *name)
{
	int index;
	index = getHash(name, ht->size);
	index = abs(index);
	if(ht->items[index] == NULL)
		return 0;
	int status = ovf_delete(&(ht->items[index]->ol), name);
	ht_clearInstances(ht, name);
	return status;
}

void ht_clearInstances(hashTable *ht, char *name)
{
	int i;
	for(i=0; i<ht->size; i++){
		if(ht->items[i] != NULL){
			ovf_clearInstance(&(ht->items[i]->ol), name);
		}
	}
}

int ht_search(hashTable *ht, char *name)
{
	int index;
	index = getHash(name, ht->size);
	index = abs(index);
	if(ht->items[index] == NULL)
		return 0;

	if(ovf_search(&(ht->items[index]->ol), name))
		return 1;
	return 0;
}

lk ht_find(hashTable *ht, char *name)
{
	int index;
	index = getHash(name, ht->size);
	index = abs(index);
	if(ht->items[index] == NULL)
		return NULL;

	lk curr;
	if((curr = ovf_find(&(ht->items[index]->ol), name)) != NULL){
		return curr;
	}
	return NULL;
}

int ht_showReceiving(hashTable *ht, char *name)
{
	int i, st, f, found=0, count = 0;
	for(i=0; i<ht->size; i++){
		if(ht->items[i] != NULL){
			int st = ovf_showReceiving(&(ht->items[i]->ol), name, &f);
			if(f == 1)
				found = 1;
			if(st == 1)
				count++;
		}
	}
	if(found == 0)
		return -1;
	if(count == 0)
		return 0;
	return 1;
}

int ht_circleFind(hashTable *ht, char *name)
{
	Stack frontier, path, explored;
	stack_initialize(&frontier);
	stack_initialize(&path);
	stack_initialize(&explored);

	int step_f = 1, flag=0, st, w, print=0;
	unsigned int count=0;
	char *curr_name = NULL;
	char *buf = NULL;
	char *temp = NULL;
	lk curr = ht_find(ht, name);

	if(curr == NULL)
		return -1;

	adj_pushAll(&curr->al, &frontier, &explored, step_f, name);			//push all successors of name wanted.
	while(!stack_isEmpty(&frontier)){
		stack_pop(&frontier, &curr_name, &step_f, &w);						//curr_name

		stack_push(&explored, curr_name, step_f, w);
		stack_push(&path, curr_name, step_f, w);

		if(strcmp(curr_name, name)==0){
			flag = 1;
			if(print == 0){
				print = 1;
				printf("Circles found:\n");
			}
			printf("%d)", count);		count++;
			stack_print(&path, name);									//"snapshot" of path
		}
		step_f++;
		curr = ht_find(ht, curr_name);								//find actual node in list
		if(strcmp(curr->name, name) != 0){
			st = adj_pushAll(&curr->al, &frontier, &explored, step_f, name);
			if(!st){														//reached end of path -> backtracking
				while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
					stack_Delete(&path);
					stack_Delete(&explored);
				}
			}
		}
		else{
			while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
				stack_Delete(&path);
				stack_Delete(&explored);
			}
		}
		if(curr_name != NULL)
			free(curr_name);
	}

	//free
	stack_freeAll(&frontier);
	stack_freeAll(&explored);
	stack_freeAll(&path);
	return flag;
}

int ht_findCircles(hashTable *ht, char *name, int min)
{
	//here we want all closed paths, as opposed to circleFind that prints only simple cycles.
	Stack frontier, path, explored;
	stack_initialize(&frontier);
	stack_initialize(&path);
	stack_initialize(&explored);

	int step_f = 1, flag=0, st, w, print=0;
	char *curr_name = NULL;
	unsigned int count=0;
	lk curr = ht_find(ht, name);
	if(curr == NULL)
		return -1;

	adj_pushAll_v2(&curr->al, &frontier, &explored, step_f, name, min);			//push all successors of name wanted.
	while(!stack_isEmpty(&frontier)){
		stack_pop(&frontier, &curr_name, &step_f, &w);

		stack_push(&explored, curr_name, step_f, w);
		stack_push(&path, curr_name, step_f, w);

		if(strcmp(curr_name, name)==0){
			flag = 1;
			if(print == 0){
				print = 1;
				printf("Circles found:\n");
			}
			printf("\n%d)", count);		count++;
			stack_print(&path, name);									//"snapshot" of path
		}
		step_f++;
		curr = ht_find(ht, curr_name);								//find actual node in list
		if(strcmp(curr->name, name) != 0){
			st = adj_pushAll_v2(&curr->al, &frontier, &explored, step_f, name, min);
			if(!st){														//reached end of path -> backtracking
				while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
					stack_Delete(&path);
					stack_Delete(&explored);
				}
			}
		}
		else{
			while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
				stack_Delete(&path);
				stack_Delete(&explored);
			}
		}
		if(curr_name != NULL)
			free(curr_name);
	}

	//free
	stack_freeAll(&frontier);
	stack_freeAll(&explored);
	stack_freeAll(&path);
	return flag;
}

int ht_getTraceflow(hashTable *ht, char *start, char *end, int max_length)
{
	//technically resembles DLS

	if(max_length == 0)
		return 1;
	Stack frontier, path, explored;
	stack_initialize(&frontier);
	stack_initialize(&path);
	stack_initialize(&explored);

	int step_f = 1, flag=0, st, w, print=0;
	char *curr_name = NULL;
	unsigned int count=0;
	lk curr = ht_find(ht, start);
	if(curr == NULL)
		return -1;

	adj_pushAll(&curr->al, &frontier, &explored, step_f, start);			//push all successors of name wanted.
	while(!stack_isEmpty(&frontier)){
		stack_pop(&frontier, &curr_name, &step_f, &w);

		stack_push(&explored, curr_name, step_f, w);
		stack_push(&path, curr_name, step_f, w);

		if(strcmp(curr_name, end)==0){
			flag = 1;
			if(print == 0){
				print = 1;
				printf("Traceflows found:\n");
			}
			printf("\n%d)", count);		count++;
			stack_print(&path, start);										//"snapshot" of path
		}
		step_f++;
		if(step_f<=max_length && strcmp(curr_name, end) != 0){
			curr = ht_find(ht, curr_name);								//find actual node in list
			st = adj_pushAll(&curr->al, &frontier, &explored, step_f, end);
			if(!st){														//reached end of path -> backtracking
				while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
					stack_Delete(&path);
					stack_Delete(&explored);
					step_f--;
				}
			}
		}
		else{
			while((!stack_isEmpty(&path)) && (stack_getStep(&path) >= stack_getStep(&frontier))){
				stack_Delete(&path);
				stack_Delete(&explored);
				step_f--;
			}
		}
		if(curr_name != NULL)
			free(curr_name);
	}

	//free
	stack_freeAll(&frontier);
	stack_freeAll(&explored);
	stack_freeAll(&path);
	return flag;
}

void ht_print(hashTable *ht)
{
	int i;
	for(i=0; i<ht->size; i++){
		if(ht->items[i] == NULL)
			continue;
		ovf_print(&(ht->items[i]->ol));
	}
}

void ht_WIF(hashTable *ht, FILE *fp)
{
	int i;
	for(i=0; i<ht->size; i++){
		if(ht->items[i] != NULL)
			ovf_WIF(&(ht->items[i]->ol), fp);
	}
}

void ht_freeAll(hashTable *ht)
{
	int i;
	for(i=0; i<ht->size; i++){
		if(ht->items[i] != NULL){
			//free(ht->items[i]->name);
			ovf_freeList(&(ht->items[i]->ol));
			free(ht->items[i]);
		}
	}
	free(ht->items);
	free(ht);
}

//list
void ovf_initialize(ovf_list *ol)
{
	ol->head = NULL;
	ol->tail = NULL;
	ol->count = 0;
}

int ovf_isEmpty(ovf_list *ol)
{
	return (ol->head == NULL);
}

void ovf_insert(ovf_list *ol, char *name)
{
	if(ovf_isEmpty(ol)){
		//first vertex to be inserted
		ol->head = (lk)malloc(sizeof(struct ovf_listNode));
		ol->head->name = strdup(name);
		adj_initialize(&(ol->head->al));
		ol->head->next = NULL;
		ol->tail = ol->head;
	}
	else{
		ol->tail->next = malloc(sizeof(struct ovf_listNode));
		ol->tail->next->name = strdup(name);
		adj_initialize(&(ol->tail->next->al));
		ol->tail->next->next = NULL;
		ol->tail = ol->tail->next;
	}
	ol->count++;
}

void ovf_insert_v2(ovf_list *ol, char *name, lk *x)
{
	//printf("\tinseting in ovf_list %s, i have count %d\n", name, ol->count);
	if(ovf_isEmpty(ol)){
		//first vertex to be inserted
		ol->head = (lk)malloc(sizeof(struct ovf_listNode));
		ol->head->name = strdup(name);
		adj_initialize(&(ol->head->al));
		ol->head->next = NULL;
		ol->tail = ol->head;
		*x = ol->head;
	}
	else{
		ol->tail->next = malloc(sizeof(struct ovf_listNode));
		ol->tail->next->name = strdup(name);
		adj_initialize(&(ol->tail->next->al));
		ol->tail->next->next = NULL;
		ol->tail = ol->tail->next;
		*x = ol->tail;
	}
	ol->count++;
}

int ovf_delete(ovf_list *ol, char *name)
{
	if(strcmp(ol->head->name, name) == 0){
		//here!
		//cont_clearInstances(cl, name);
		free(ol->head->name);							//free name
		adj_freelist(&(ol->head->al));					//free adj_list of this node
		lk temp = ol->head;							//with temp's help make the step
		ol->head = ol->head->next;
		free(temp);										//free node.
		ol->count--;
		return 1;
	}
	else{
		lk curr = ol->head->next;
		lk prev = ol->head;
		while(curr != NULL){
			if(strcmp(curr->name, name) == 0){
				//here!
				//cont_clearInstances(cl,name);
				if(ol->tail == curr){					//fix tails position.
					ol->tail = prev;
				}
				prev->next = curr->next;				//same steps as before .^.
				free(curr->name);
				adj_freelist(&(curr->al));
				free(curr);
				ol->count--;
				return 1;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return 0;
}

void ovf_clearInstance(ovf_list *ol, char *name)
{
	lk curr = ol->head;
	while(curr != NULL){
		adj_delete(&curr->al, name);
		curr = curr->next;
	}
}

int ovf_showReceiving(ovf_list *ol, char *name, int *f)
{
	lk curr = ol->head;
	int flag=0, found = 0, st;
	while(curr != NULL){
		if(strcmp(curr->name, name) == 0)
			found = 1;
		if((st = adj_printEdges(&curr->al, name, curr->name))== 1){		//everytime name is encountered in currents adjacency list, the edge is printed.
			flag=1;
		}
		curr = curr->next;
	}
	*f = found;
	return flag;
}

int ovf_search(ovf_list *ol, char *name)
{
	lk curr = ol->head;
	while(curr != NULL){
		if(strcmp(curr->name, name)==0){
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

lk ovf_find(ovf_list *ol, char *name)
{
	lk curr = ol->head;
	while(curr != NULL){
		if(strcmp(curr->name, name)==0){
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

void ovf_print(ovf_list *ol)
{
	if(ovf_isEmpty(ol))
		return;
	lk curr = ol->head;
	while(curr != NULL){
		printf("%s", curr->name);
		adj_print(&curr->al);
		printf("\n|\nv\n");
		//printf("%s, ", curr->name);
		curr = curr->next;
	}
}

void ovf_WIF(ovf_list *ol, FILE *fp)
{
	if(ovf_isEmpty(ol))
		return;
	lk curr = ol->head;
	while(curr != NULL){
		fprintf(fp, "|%s|\n", curr->name);
		adj_WIF(&curr->al, fp);
		fprintf(fp, "\n\n");
		curr = curr->next;
	}
}

void ovf_freeList(ovf_list *ol)
{
	if(!ovf_isEmpty(ol)){
		lk curr = ol->head;
		lk temp = NULL;
		while(curr != NULL){
			free(curr->name);
			adj_freelist(&curr->al);
			temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
}

//////////////////////////////////////////////////////////

void graph_initialize(Graph *G, int size)
{
	ht_initialize(&(G->ht), size);
	G->count = 0;
}

int graph_isEmpty(Graph *G)
{
	return ht_isEmpty(G->ht);
}

lk graph_findVertex(Graph *G, char *name)
{
	lk node = ht_find(G->ht, name);
	return node;
}

int graph_addVertex_Sr(Graph *G, char *name)
{
	if(!ht_search(G->ht, name)){
		ht_insert(G->ht, name);
		G->count++;
		return 1;
	}
	return 0;
}

lk graph_addVertex(Graph *G, char *name)
{
	//same as above without searching + returns node
	lk x;
	ht_insert_v2(G->ht, name, &x);
	G->count++;
	return x;
}

void graph_addEdge(Graph *G, char *name, char *to, int weight)
{
	lk node = graph_findVertex(G, name);			//get wanted vertex.

	if(node == NULL)
		node = graph_addVertex(G, name);

	lk to_node = graph_findVertex(G, to);			//check if "to" vertex exists
	if(to_node == NULL)
		to_node = graph_addVertex(G, to);


	if(!adj_search(&(node->al), to)){
		//current vertex doesnt already point to "to" vertex, make new entry in adjacency list
		adj_insert(&(node->al), to, weight);
	}
	else{
		//current vertex already points, so add to same list of that node
		adj_insertSame(&(node->al), to, weight);
	}
}

void graph_print(Graph *G)
{
	if(graph_isEmpty(G)){
		printf("Graph is empty.\n");
		return;
	}
	ht_print(G->ht);
}

void graph_freeAll(Graph *G)
{
	ht_freeAll(G->ht);
}

int graph_deleteVertex(Graph *G, char *name)
{
	G->count--;
	int status = ht_delete(G->ht, name);
	return status;
}

int graph_deleteEdge(Graph *G, char *name, char *to, int weight, int flag)
{
	//returns: 0 if "name" vertex doesn't exist, -1 if to vertex doesn't exist, -2 if "name" and "to" aren't connected,
	//-3 if "name" and "to" aren't connected with edge weighted "weight", 1 if all ok.

	lk node = graph_findVertex(G, name);			//get wanted vertex.
	lk to_node = graph_findVertex(G, to);			//check if "to" vertex exists
	if(node == NULL)
		return 0;
	else if(to_node == NULL)
		return -1;

	int status = adj_removeEdge(&(node->al), to, weight, flag);
	if(status == -1)
		return -2;
	else if(status == 0)
		return -3;
	else
		return 1;
}

int graph_modifyWeight(Graph *G, char *name, char *to, int weight, int nweight)
{
	lk node = graph_findVertex(G, name);			//get wanted vertex.
	lk to_node = graph_findVertex(G, to);			//check if "to" vertex exists
	if(node == NULL)
		return 0;
	else if(to_node == NULL)
		return -1;

	int status = adj_modifyWeight(&(node->al), to, weight, nweight);
	if(status == -1)
		return -2;
	else if(status == 0)
		return -3;
	else
		return 1;
}

int graph_showReceiving(Graph *G, char *name)
{
	return ht_showReceiving(G->ht, name);
}

int graph_circleFind(Graph *G, char *name)
{
	return ht_circleFind(G->ht, name);
}

int graph_findCircles(Graph *G, char *name, int min)
{
	return ht_findCircles(G->ht, name, min);
}

int graph_getTraceflow(Graph *G, char *start, char *end, int max_length)
{
	return ht_getTraceflow(G->ht, start, end, max_length);
}
