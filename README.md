## Directed Multigraph

Implementation of **dynamic** directed multigraph in **C** that monitors the money transfers between the users and helps in detecting multi-triangular transactions as well as user group setups.<br>

Example of a directed multigraph:
![graph](https://github.com/errikosg/Directed_Multigraph/blob/master/img/graph.png)<br>

After the graph initialization, the user **interacts** with the program and can choose one of the following commands:
1. **i(nsert) Ni**: insert new node Ni
2. **(i)n(sert) Ni Nj weight**: insert new edge between Ni, Nj with given weight. Nodes can be new to the graph.
3. **d(elete) Ni**: delete node Ni as well as all in/out edges.
4. **(de)l(ete) Ni Nj weight**: delete edge between Ni,Nj with given weight. If there are more than 1 correct edges, one of them is deleted. If weight isn't specified, all are deleted.
5. **m(odify) Ni Nj weight nweight**: change weight to new value nweight. If there are more than 1 correct edges, change the first encountered.
6. **r(eceiving) Ni**: show all incoming transactions of node Ni.
7.  **c(irclefind) Ni**: find all simple cycles of node Ni.
8. **f(indcircles) Ni k**: find all cyclical transactions of node Ni, given that every edge has at least given weight.
9. **t(raceflow) Ni Nj l**: find possible "traceflows"(paths) starting Ni and ending Nj, given that the path length is at maximum l.
10. **e(xit)**: program clears memory and exits.

#### Compile and Run
A makefile is included. To run: **./mygraph -i inputfile -o outfile**<br>where inputfile is formatted according to the files in /datasets and outfile contains the graph state at the end of the user queries.

