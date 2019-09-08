#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adjacencyList.h"

void adj_initialize(adj_list *al)
{
	al->count=0;
	al->head = NULL;
	al->tail = NULL;
}

int adj_isEmpty(adj_list *al)
{
	return (al->head==NULL);
}

void adj_insert(adj_list *al, char *name, int weight)
{
	//I assume that vertex to be inserted is unique till this moment - otherwise it will be inserted in same list...
	if(adj_isEmpty(al)){
		//first vertex to be inserted

		al->head = (lnk)malloc(sizeof(struct vertex));
		al->head->next = NULL;
		al->head->name = strdup(name);
		al->head->weight = weight;
		adj_initialize(&al->head->same);
		al->tail = al->head;
	}
	else{
		al->tail->next = malloc(sizeof(struct vertex));
		al->tail->next->name = strdup(name);
		al->tail->next->weight = weight;
		adj_initialize(&al->tail->next->same);
		al->tail->next->next = NULL;
		al->tail = al->tail->next;
	}
	al->count++;
}

void adj_insertSame(adj_list *al, char *name, int weight)
{
	//I assume a same vertex exists
	lnk x = adj_find(al, name, NULL);
	if(x == NULL)
		return;		//!
	adj_insert(&x->same, name, weight);
}

void adj_delete(adj_list *al, char *name)
{
	//works only for command(d): deletes every instance
	if(adj_isEmpty(al))
		return;
	if(strcmp(al->head->name, name) == 0){
		//deleting first entry of adjacency list
		free(al->head->name);
		adj_freelist(&(al->head->same));
		lnk temp = al->head;
		al->head = al->head->next;
		free(temp);
	}
	else{
		lnk curr = al->head->next;
		lnk prev = al->head;
		while(curr != NULL){
			if(strcmp(curr->name, name) == 0){
				if(al->tail == curr){					//fix tails position.
					al->tail = prev;
				}
				prev->next = curr->next;
				free(curr->name);
				adj_freelist(&(curr->same));
				free(curr);
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	al->count--;
}

void adj_deleteHead(adj_list *al)
{
	if(adj_isEmpty(al))
		return;
	//deleting first entry of adjacency list
	free(al->head->name);
	adj_freelist(&(al->head->same));
	lnk temp = al->head;
	al->head = al->head->next;
	free(temp);
}

int adj_removeEdge(adj_list *al, char *to, int weight, int flag)
{
	//returns 0 only if doesn't exist edge with given weight
	//printf("--Was given: %s, %d and flag %d\n", to, weight, flag);

	lnk prev = NULL;
	lnk curr = adj_find(al, to, &prev);

	if(curr == NULL)
		return -1;

	if(flag == 1){
		if(curr->weight != weight && !adj_searchWeight(&curr->same, weight))		//edge with given weight and name doesn't exist.
			return 0;

		//was given a specified weight, will delete one edge to vertex.
		if(curr->weight == weight){
			//curr has to be deleted
			if(!adj_isEmpty(&curr->same)){
				//if it has entries in same list, makes a swap.
				int temp = curr->same.head->weight;
				curr->same.head->weight = curr->weight;
				curr->weight = temp;
				adj_deleteHead(&curr->same);
			}
			else{
				//same-list empty, deletes same node.
				if(curr == al->head)
					adj_deleteHead(al);
				else{
					if(al->tail == curr){					//fix tails position.
						al->tail = prev;
					}
					prev->next = curr->next;
					free(curr->name);
					adj_freelist(&(curr->same));
					free(curr);
				}
			}
		}
		else{
			adj_delete(&curr->same, curr->name);
		}
	}
	else if(flag == 0){
		//was not given weight, delete all of "to" node
		if(curr == al->head){
			adj_freelist(&(curr->same));
			adj_deleteHead(al);
		}
		else{
			if(al->tail == curr){					//fix tails position.
				al->tail = prev;
			}
			prev->next = curr->next;
			free(curr->name);
			adj_freelist(&(curr->same));
			free(curr);
		}
	}
	return 1;
}

int adj_modifyWeight(adj_list *al, char *to, int weight, int nweight)
{
	lnk prev;
	lnk curr = adj_find(al, to, &prev);
	lnk x;

	if(curr == NULL)
		return -1;
	if(curr->weight == weight)
		curr->weight = nweight;
	else if((x = adj_findWeight(&curr->same, weight)) != NULL)
		x->weight = nweight;
	else
		return 0;
}

int adj_printEdges(adj_list *al, char *name, char *v_name)
{
	if(!adj_isEmpty(al)){
		lnk curr = al->head;
		int flag=0;
		while(curr != NULL){
			if(strcmp(curr->name, name) == 0){								//if curr node is the one we are searching for
				printf("|%s| --%d--> |%s|\n", v_name, curr->weight, name);
				flag = 1;
			}
			adj_printEdges(&curr->same, name, v_name);						//check also same list for multi edges
			curr = curr->next;
		}
		return flag;
	}
	return 0;
}

int adj_pushAll(adj_list *al, Stack *fr, Stack *exp, int step, char *name)
{
	lnk curr = al->head;
	int flag = 0;
	while(curr != NULL){
		if((strcmp(curr->name, name) == 0) || !stack_search(exp, curr->name)){
			stack_push(fr, curr->name, step, curr->weight);
			flag = 1;
		}
		if(!adj_isEmpty(&curr->same)){
			//search in same list
			adj_pushAll(&curr->same, fr, exp, step, name);
			flag = 1;
		}
		curr = curr->next;
	}
	return flag;
}

int adj_pushAll_v2(adj_list *al, Stack *fr, Stack *exp, int step, char *name, int min)
{
	lnk curr = al->head;
	int flag = 0;
	while(curr != NULL){
		if(((strcmp(curr->name, name) == 0) || !stack_searchWeight(exp,curr->name,curr->weight)) && curr->weight>min){
			stack_push(fr, curr->name, step, curr->weight);
			flag = 1;
		}
		if(!adj_isEmpty(&curr->same)){
			//search in same list
			adj_pushAll_v2(&curr->same, fr, exp, step, name, min);
			flag = 1;
		}
		curr = curr->next;
	}
	return flag;
}

lnk adj_find(adj_list *al, char *name, lnk *x)
{
	//(*x) serves as previous, fixes pointer given.
	lnk curr = al->head;
	lnk prev = NULL;
	while(curr != NULL){
		if(strcmp(curr->name, name)==0){
			if (x != NULL)
				*x = prev;
			return curr;
		}
		prev = curr;
		curr = curr->next;
	}
	return NULL;
}

int adj_search(adj_list *al, char *name)
{
	//simple search function to find if vertex(with name) exists in adjacency list.
	lnk curr = al->head;
	while(curr != NULL){
		if(strcmp(curr->name, name)==0){
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

lnk adj_findWeight(adj_list *al, int weight)
{
	lnk curr = al->head;
	while(curr != NULL){
		if(curr->weight == weight){
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}


int adj_searchWeight(adj_list *al, int weight)
{
	//simple search function to find if vertex(with name) exists in adjacency list.
	lnk curr = al->head;
	while(curr != NULL){
		if(curr->weight == weight){
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

void adj_print(adj_list *al)
{
	lnk curr = al->head;
	while(curr != NULL){
		printf("-> %s(%d)", curr->name, curr->weight);
		if(!adj_isEmpty(&curr->same)){
			printf(" *[");
			adj_print2(&curr->same);
			printf("]");
		}
		curr = curr->next;
	}
	//printf("\n");
}

void adj_print2(adj_list *al)
{
	lnk curr = al->head;
	while(curr != NULL){
		printf("%d ", curr->weight);
		curr = curr->next;
	}
}

void adj_WIF(adj_list *al, FILE *fp)
{
	lnk curr = al->head;
	while(curr != NULL){
		fprintf(fp, "\t--%d--> |%s|\n", curr->weight, curr->name);
		curr = curr->next;
	}
}

void adj_freelist(adj_list *al)
{
	if(!adj_isEmpty(al)){
		lnk curr = al->head;
		lnk temp = NULL;
		while(curr != NULL){
			free(curr->name);
			adj_freelist(&curr->same);
			temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
}

