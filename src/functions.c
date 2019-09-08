#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "graph.h"

void clearScreen()
{
	int i;
	for(i=0; i<10; i++)
		printf("\n");
}

int mkOutFile(Graph *G, char *doc)
{
	FILE *fp;
	if((fp = fopen(doc, "w")) == NULL){
		perror("fopen");
		return 0;
	}

	ht_WIF(G->ht, fp);
	fclose(fp);
	return 1;
}

int getLines(Graph *G, char *file)
{
	FILE *fp;
	if((fp = fopen(file, "r")) == NULL){
		perror("fopen");
		return -1;
	}
	
	int lines=0, i, counter;
	char c;
	while(c = fgetc(fp)){
		if(c == EOF)
			break;
		if(c == '\n')
			lines++;
	}
	fclose(fp);
	return lines;
}

int buildGraph(Graph *G, char *file, int lines)
{
	//function that build the graph based upon the given file.
	FILE *fp;
	if((fp = fopen(file, "r")) == NULL){
		perror("fopen");
		return -1;
	}
	char *buff;
	int i, counter;
	char c;

	for(i=0; i<lines; i++){
		counter=0;
		while((c = (char)fgetc(fp)) != '\n'){
			counter++;
		}
		if(counter==0){
			fprintf(stderr, "Empty line. Error\n");
			fclose(fp);
			return -1;
		}
		counter++;
		fseek(fp, -counter, SEEK_CUR);
		buff = malloc(counter*sizeof(char)+1);
		fgets(buff, counter, fp);						
		buff[counter-1] = '\0';
		fseek(fp, 1, SEEK_CUR);
		//printf("%s\n", buff);

		char *name; char *to; int w;
		parseString(&name, &to, &w, buff);
		graph_addEdge(G, name, to, w);
		free(buff);
	}

	fclose(fp);
}

void parseString(char **name, char **to, int *w, char *buff)
{
	//get name, to from line read from file
	*name = NULL;
	*to = NULL;
	char *token;
	token = strtok(buff, " \t");
	*name = token;
	token = strtok(NULL, " \t");
	*to = token;
	token = strtok(NULL, " \t");
	*w = atoi(token);
}

void printCommands()
{
	printf("Supported commands:\n");
	printf("\ti Ni: Insert vertex |Ni| in graph.\n");
	printf("\tn Ni Nj weight: Insert edge between vertices |Ni|, |Nj| with weight -weight-.\n");
	printf("\td Ni: Delete vertex |Ni| from graph.\n");
	printf("\tl Ni Nj weight: Delete edge between vertices |Ni|, |Nj| with weight -weight-.\n");
	printf("\tm Ni Nj weight nweight: Modify weight of edge between |Ni|, |Nj| with weight -weight-, giving new weight -nweight-\n");
	printf("\tr Ni: Print all incoming edges and corresponding vertices of vertex |Ni|\n");
	printf("\tc Ni: Print all simple cycles in graph that include vertex |Ni|\n");
	printf("\tf Ni weight: Print all closed walks (complex cycles) in graph that include vertex |Ni|, but with minimum step cost = -weight-.\n");
	printf("\tt Ni Nj l: Find all paths with max length -l- from vertex |Ni| to vertex |Nj|.\n");
	printf("\te: Clear space and exit program.\n");
}
