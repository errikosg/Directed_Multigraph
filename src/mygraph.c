#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include "graph.h"
#include "functions.h"

#define PERMS 0644

void int_handler(int);

int main(int argc, char *argv[])
{
	int i, j, out_flag=0, in_flag=0;
	char *out_doc; char *in_doc;			//out_doc = file that output will be written / in_doc = file to read from
	struct timeval st, et;

    //Check input given
	for(i=1; i<argc; i++){
		if(strcmp(argv[i], "-i") == 0){
			in_flag = 1;
			if(argv[i+1] == NULL){
				printf("Error: wrong arguments given.\n");
				exit(1);
			}
			in_doc = strdup(argv[i+1]);
		}
		else if(strcmp(argv[i], "-o") == 0){
			out_flag = 1;
			if(argv[i+1] == NULL){
				printf("Error: wrong arguments given.\n");
				exit(1);
			}
			out_doc = strdup(argv[i+1]);
		}
	}
	//show Error messages in case of incomplete arguments
	if(in_flag == 0)
		fprintf(stderr, "! Warning: Input file not given, graph will not be created.\n");
	if(out_flag == 0)
		fprintf(stderr, "! Warning: Output file not given, terminating state will not be written in a file.\n");

	printf("good\n");

	Graph G;
	//graph_initialize(&G, 20);

	//read from file if given
	if(in_flag == 1){
		int lines = getLines(&G, in_doc);
		int l = 3*lines/4;
		graph_initialize(&G, l);
		buildGraph(&G, in_doc, lines);
	}
	else
		graph_initialize(&G, 100);

	
	//begin interaction with user
	char *cmd;
	char **arg;
	int args;
	cmd = (char *)malloc(150*sizeof(char));
	arg = (char **)malloc(6*sizeof(char *));

	while(1){
		//clearScreen();
		printf("\nMygraph>>");
		if(!fgets(cmd, 150, stdin)){
			fprintf(stderr, "Error in getting command, exiting...\n");
			break;
		}
		while(strcmp(cmd, "\n")==0){
			printf("Mygraph>>");
			fgets(cmd, 150, stdin);
		}

		//parse command
		size_t length = strlen(cmd);
		if(cmd[length-1]=='\n')
			cmd[length-1] = '\0';
		char *token;
		token = strtok(cmd, " ");
		args=0;

		if(strcmp(token, "e")==0){
			//exit command
			break;
		}
		else if(strcmp(token, "i") == 0){
			//insert Ni command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 2)
				fprintf(stderr, "Error: Invalid arguments of i(nsert) Ni\n");
			else{
				int status = graph_addVertex_Sr(&G, arg[0]);
				if(status)
					printf("Vertex |%s| inserted.\n", arg[0]);
				else
					printf("Vertex |%s| exists.\n", arg[0]);
			}
		}
		else if(strcmp(token, "n") == 0){
			//insert Ni Nj command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 4)
				fprintf(stderr, "Error: Invalid arguments of (i)n(sert) Ni Nj weight\n");
			else{
				graph_addEdge(&G, arg[0], arg[1], atoi(arg[2]));
				printf("Inserted edge: |%s| --%d--> |%s|\n", arg[0], atoi(arg[2]), arg[1]);
			}
		}
		else if(strcmp(token, "d")==0){
			//delete Ni command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 2)
				fprintf(stderr, "Error: Invalid arguments of d(elete) Ni\n");
			else{
				int status = graph_deleteVertex(&G, arg[0]);
				if(!status)
					printf("Error: vertex |%s| doesn't exist.\n", arg[0]);
				else
					printf("Deleted vertex |%s|.\n", arg[0]);
			}
		}
		else if(strcmp(token, "l") == 0){
			//delete Ni Nj weight command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args !=4 && args != 3)
				fprintf(stderr, "Error: invalid arguments of (d)e(lete) Ni Nj weight.\n");
			else{
				int status;
				if(args == 3){
					status = graph_deleteEdge(&G,  arg[0], arg[1], 0, 0);					//no weight
				}
				else{
					status = graph_deleteEdge(&G,  arg[0], arg[1], atoi(arg[2]), 1);		//weight
				}

				switch(status){
					case 0:
						printf("Vertex |%s| doesn't exist!\n", arg[0]); break;
					case -1:
						printf("Vertex |%s| doesn't exist!\n", arg[1]); break;
					case -2:
						printf("Edge between |%s| and |%s| doesn't exists.\n", arg[0], arg[1]); break;
					case -3:
						if(args==4)
							printf("Edge between |%s| and |%s| with weight %d doesn't exist.\n", arg[0], arg[1], atoi(arg[2]));
						break;
					default:
						if(args==4)
							printf("Deleted edge: |%s| --%d--> |%s|\n", arg[0], atoi(arg[2]), arg[1]);
						else
							printf("Deleted all edges from |%s| to |%s|\n", arg[0], arg[1]);
				}
			}
		}
		else if(strcmp(token, "m") == 0){
			//m(odify) Ni Nj weight nweight command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>5) break;
			}
			if(args != 5)
				fprintf(stderr, "Error: Invalid arguments of m(odify) Ni Nj weight nweight.\n");
			else{
				int status = graph_modifyWeight(&G, arg[0], arg[1], atoi(arg[2]), atoi(arg[3]));
				switch(status){
					case 0:
						printf("Vertex |%s| doesn't exist!\n", arg[0]); break;
					case -1:
						printf("Vertex |%s| doesn't exist!\n", arg[1]); break;
					case -2:
						printf("Edge between |%s| and |%s| doesn't exists.\n", arg[0], arg[1]); break;
					case -3:
						printf("Edge between |%s| and |%s| with weight %d doesn't exist.\n", arg[0], arg[1], atoi(arg[2])); break;
					default:
						printf("Modified |%s| --%d--> |%s|\n", arg[0], atoi(arg[3]), arg[1]);
				}
			}
		}
		else if(strcmp(token, "r") == 0){
			//receiving Ni command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 2)
				fprintf(stderr, "Error: Invalid arguments for command r(eceiving Ni)\n");
			else{
				int status = graph_showReceiving(&G, arg[0]);
				if(status == 0)
					printf("No receiving edges %s\n", arg[0]);
				else if(status == -1)
					printf("Error: Vertex |%s| doesn't exist!\n", arg[0]);
			}
		}
		else if(strcmp(token, "c")==0){
			//c(irclefind Ni) command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 2)
				fprintf(stderr, "Error: Invalid arguments of command c(irclefind) Ni\n");
			else{
				//gettimeofday(&st,NULL);						//time measurement

				int status = graph_circleFind(&G, arg[0]);

				/*gettimeofday(&et,NULL);
				double elapsed = (et.tv_sec - st.tv_sec)+((et.tv_usec - st.tv_usec)*0.000001);
				printf("\n--Time elapsed: %f seconds\n", elapsed);*/

				if(status == 0)
					printf("No circles found for %s\n", arg[0]);
				else if(status == -1)
					printf("Vertex |%s| doesn't exist\n", arg[0]);
			}
		}
		else if(strcmp(token, "f") == 0){
			//f(indcircles) Ni k command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 3)
				fprintf(stderr, "Error: Invalid arguments for f(indcircles) Ni k command\n");
			else{
				//gettimeofday(&st,NULL);						//time measurement

				int status = graph_findCircles(&G, arg[0], atoi(arg[1]));

				/*gettimeofday(&et,NULL);
				double elapsed = (et.tv_sec - st.tv_sec)+((et.tv_usec - st.tv_usec)*0.000001);
				printf("\n--Time elapsed: %f seconds\n", elapsed);*/

				if(status == 0)
					printf("No closed walks found for %s\n", arg[0]);
				else if(status == -1)
					printf("Vertex |%s| doesn't exist\n", arg[0]);
			}
		}
		else if(strcmp(token, "t") == 0){
			//t(raceflow) Ni Nj l command
			while(token != NULL){
				token = strtok(NULL, " ");
				arg[args] = token;
				args++;
				if(args>4) break;
			}
			if(args != 4)
				fprintf(stderr, "Error: Invalid arguments for t(raceflow) Ni Nj k command\n");
			else{
				//gettimeofday(&st,NULL);						//time measurement

				int status = graph_getTraceflow(&G, arg[0], arg[1], atoi(arg[2]));

				/*gettimeofday(&et,NULL);
				double elapsed = (et.tv_sec - st.tv_sec)+((et.tv_usec - st.tv_usec)*0.000001);
				printf("\n--Time elapsed: %f seconds\n", elapsed);*/

				if(status == 0)
					printf("No traceflow found for %s\n", arg[0]);
				else if(status == -1)
					printf("Vertex |%s| doesn't exist\n", arg[0]);
			}
		}
		else if(strcmp(token, "/print") == 0){
			graph_print(&G);
		}
		else if(strcmp(token, "/cmds") == 0){
			printCommands();
		}
		else{
			printf("Unknown command: %s\n", token);
		}
	}
	

	//exit / write in outputfile / free used space
	if(out_flag == 1){
		int res = mkdir("OutputFiles", 0755);
		if(res >= 0)
			printf("Created directory.\n");
		printf("Writing to output file...\n");

		char *dest = malloc((strlen(out_doc)+strlen("OutputFiles"))*sizeof(char)+5);
		strcpy(dest, "./OutputFiles/");
		strcat(dest, out_doc);

		int st = mkOutFile(&G, dest);
		if(!st)
			fprintf(stderr, "Error: Failed to write in output file.\n");

		free(dest);
		free(out_doc);
	}
	printf("Clearing memory...\n");
	if(in_flag == 1)
		free(in_doc);
	free(cmd);
	free(arg);
	graph_freeAll(&G);

	printf("\nGoodbye.\n");
	exit(0);
}
