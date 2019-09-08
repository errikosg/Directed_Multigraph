OBJS	= mygraph.o functions.o graph.o adjacencyList.o stack.o
SOURCE	= mygraph.c functions.c graph.c adjacencyList.c stack.c
HEADER	= functions.h graph.h adjacencyList.h stack.h
OUT		= mygraph
CC		= gcc
FLAGS	= -g -c

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

#create-compile individual files.
mygraph.o: mygraph.c
	$(CC) $(FLAGS) mygraph.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

graph.o: graph.c
	$(CC) $(FLAGS) graph.c

adjacencyList.o: adjacencyList.c
	$(CC) $(FLAGS) adjacencyList.c

stack.o: stack.c
	$(CC) $(FLAGS) stack.c

#clean place
clean:
	rm -f $(OBJS) $(OUT)

#acounting
count:
	wc $(SOURCE) $(HEADER)
