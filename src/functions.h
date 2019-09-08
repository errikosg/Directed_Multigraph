#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "graph.h"

//interface of generic functions used by mygraph.c

void clearScreen();
int mkOutFile(Graph *, char *);							//creates and write to output file given
int getLines(Graph *, char *);
int buildGraph(Graph *, char *, int);						//builds graph according to input file given
void parseString(char **, char **, int *, char *);		//parses strings in input file - (name)tab(name)tab(weight)
void printCommands();									//prints all commands supported by the program

#endif
