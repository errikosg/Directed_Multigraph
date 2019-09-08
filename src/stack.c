#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void stack_initialize(Stack *S)
{
	S->Itemlist = NULL;
	S->count = 0;
}

int stack_isEmpty(Stack *S)
{
	return (S->Itemlist == NULL);
}

void stack_push(Stack *S, char *name, int step, int weight)
{
	//push at beginning(head).
	StackNode *temp = malloc(sizeof(StackNode));
	temp->next = S->Itemlist;
	temp->prev = NULL;
	temp->name = strdup(name);
	temp->step = step;
	temp->weight = weight;

	if(!stack_isEmpty(S))
		S->Itemlist->prev = temp;
	S->Itemlist = temp;
	S->count++;
	if(S->count == 1)
		S->end = S->Itemlist;
}

void stack_pop(Stack *S, char **x, int *s, int *w)
{
	//pop from beginning(head).
	if(!stack_isEmpty(S)){
		StackNode *temp;
		temp = S->Itemlist;
		(*x) = strdup(temp->name);
		*s = temp->step;
		*w = temp->weight;
		S->Itemlist = temp->next;
		//S->Itemlist->prev = NULL;
		free(temp->name);
		free(temp);
		S->count--;
		if(stack_isEmpty(S))
			S->end = NULL;
	}
}

void stack_Delete(Stack *S)
{
	if(!stack_isEmpty(S)){
		StackNode *temp;
		temp = S->Itemlist;
		S->Itemlist = temp->next;
		//S->Itemlist->prev = NULL;
		free(temp->name);
		free(temp);
		S->count--;
		if(stack_isEmpty(S))
			S->end = NULL;
	}
}

void stack_freeAll(Stack *S)
{
	while(!stack_isEmpty(S)){
		StackNode *temp = S->Itemlist;
		S->Itemlist = temp->next;
		free(temp->name);
		free(temp);
	}
}
void stack_printNormal(Stack *S)
{
	if(stack_isEmpty(S))
		return;
	StackNode *temp = S->Itemlist;
	while(temp != NULL){
		printf("%s-> ", temp->name);
		temp = temp->next;
	}
	printf("\n");
}


void stack_print(Stack *S, char *name)
{
	if(stack_isEmpty(S))
		return;

	printf("|%s|", name);
	StackNode *temp = S->end;
	while(temp != NULL){
		printf(" --%d--> |%s|", temp->weight, temp->name);
		temp = temp->prev;
	}
	printf("\n\n");
}

int stack_search(Stack *S, char *name)
{
	StackNode *temp = S->Itemlist;
	while(temp != NULL){
		if(strcmp(temp->name, name) == 0)
			return 1;
		temp = temp->next;
	}
	return 0;
}

int stack_searchWeight(Stack *S, char *name, int weight)
{
	StackNode *temp = S->Itemlist;
	while(temp != NULL){
		if((strcmp(temp->name, name) == 0) && (temp->weight == weight))
			return 1;
		temp = temp->next;
	}
	return 0;
}

int stack_getStep(Stack *S)
{
	if(!stack_isEmpty(S))
		return S->Itemlist->step;
	else
		return -1;
}

