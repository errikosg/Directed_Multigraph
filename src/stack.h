#ifndef STACK_H
#define STACK_H


//A simple stack interface implemented with linked list.
//vasismeno stis simeiwseis k.Koumparaki.
typedef struct StackNodeTag{
	char *name;
	struct StackNodeTag *next;
	struct StackNodeTag *prev;
	int step; 					//crucial for dfs.
	int weight;
}StackNode;

typedef struct{
	StackNode *Itemlist;
	StackNode *end;
	int count;
}Stack;


//functions
void stack_initialize(Stack *);
int stack_isEmpty(Stack *);
void stack_push(Stack *, char *, int, int);
void stack_pop(Stack *, char **, int *, int *);
void stack_Delete(Stack *);						//pops 1st value, but deletes it.
void stack_freeAll(Stack *);
void stack_printNormal(Stack *);				//print reversed
void stack_print(Stack *, char *);
int stack_search(Stack *, char *);				//like a list
int stack_searchWeight(Stack *, char *, int);	//same as search, but checks weight also - (for edges!)
int stack_getStep(Stack *);

#endif
