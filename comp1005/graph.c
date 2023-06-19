#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

/* initialise an empty graph */
/* return pointer to initialised graph */
Graph *init_graph(void)
{
    return initialise_linked_list();
}

/* release memory for graph */
void free_graph(Graph *graph)
{
    if(!graph)
    {
        fprintf(stderr,"warning: Unable to free graph\n");
        return;
    }
    free(graph);
}

/* initialise a vertex */
/* return pointer to initialised vertex */
Vertex *init_vertex(int id)
{
    Vertex *vrt;
    vrt = (Vertex*)malloc(sizeof(Vertex));
    vrt->edges=NULL;
    vrt->id=id;
    return vrt;
}

/* release memory for initialised vertex */
void free_vertex(Vertex *vertex)
{
    if(!vertex)
    {
        fprintf(stderr,"warning: Unable to free vertex\n");
        return;
    }
    
    free(vertex);
}

/* initialise an edge. */
/* return pointer to initialised edge. */
Edge *init_edge(void)
{
    Edge *edg;
    edg=(Edge*)malloc(sizeof(Edge));
    edg->vertex=NULL;
    edg->weight=0.0;
    return edg;
}

/* release memory for initialised edge. */
void free_edge(Edge *edge)
{
    if(!edge)
    {
        fprintf(stderr,"warning: Unable to free edge\n");
        return;
    }
    free(edge);
}

/* remove all edges from vertex with id from to vertex with id to from graph. */
void remove_edge(Graph *graph, int from, int to)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to remove edge\n");
        fprintf(stderr,"warning: Unable to find vertex\n");
        return;
    }
}

/* remove all edges from vertex with specified id. */
void remove_edges(Graph *graph, int id)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to remove edges\n");
        return;
    }
}

/* output all vertices and edges in graph. */
/* each vertex in the graphs should be printed on a new line */
/* each vertex should be printed in the following format: */
/* vertex_id: edge_to_vertex[weight] edge_to_vertex[weight] ... */
/* for example: */
/* 1: 3[1.00] 5[2.00] */
/* indicating that vertex id 1 has edges to vertices 3 and 5 */
/* with weights 1.00 and 2.00 respectively */
/* weights should be output to two decimal places */
void print_graph(Graph *graph)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to print graph\n");
        return;
    }
}

/* find vertex with specified id in graph. */
/* return pointer to vertex, or NULL if no vertex found. */
Vertex *find_vertex(Graph *graph, int id)
{
    Vertex *vrt;
    Node *current;
    int found=0;
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to find vertex\n");
        return NULL;
    }
    current = graph->head;
    while(current)
    {
        if(current->data!=NULL)
        {
            vrt=current->data;
            if(vrt->id==id)
            {
                found=1;
                break;
            }
        }
        current=current->next;
    }
    if(found==0)
    {
        fprintf(stderr,"warning: Unable to find vertex\n");
        return NULL;
    }
    return vrt;
}

/* create and add vertex with specified id to graph. */
/* return pointer to vertex or NULL if an error occurs. */
/* if vertex with id already exists, return pointer to existing vertex. */
Vertex *add_vertex(Graph *graph, int id)
{
    Vertex *vrt;
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to add vertex\n");
        return NULL;
    }
    vrt=init_vertex(id);
    append_linked_list(graph,vrt);
    return vrt;
}

/* remove vertex with specified id from graph. */
/* remove all edges between specified vertex and any other vertices in graph. */
void remove_vertex(Graph *graph, int id)
{
    Vertex *vrt;
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to remove vertex\n");
        fprintf(stderr,"warning: Unable to find vertex\n");
        return;
    }
    vrt = find_vertex(graph,id);
    if(vrt==NULL)
        return;
    free_vertex(vrt);
}

/* add directed edge with specified weight between vertex with id from */
/* to vertex with id to. */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple edges between the same pair of vertices are allowed. */
/* return pointer to edge, or NULL if an error occurs found. */
Edge *add_edge(Graph *graph, int from, int to, double weight)
{
    Vertex *src,*dest;
    Edge *createdEdge;
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to add edge\n");
        return NULL;
    }
    src=find_vertex(graph,from);
    dest=find_vertex(graph,to);
    if(src==NULL)
    {
        src=add_vertex(graph,from);
    }
    if(dest==NULL)
    {
        dest=add_vertex(graph,to);
    }
    createdEdge=init_edge();
    createdEdge->vertex=dest;
    createdEdge->weight=weight;
    append_linked_list(src->edges,createdEdge);
    return createdEdge;
}

/* add two edges to graph, one from vertex with id from to vertex with id to, */
/* and one from vertex with id to to vertex with id from. */
/* both edges should have the same weight */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple vertices between the same pair of vertices are allowed. */
void add_edge_undirected(Graph *graph, int from, int to, double weight)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to add undirected edge\n");
        return;
    }
    add_edge(graph,from,to,weight);
    add_edge(graph,to,from,weight);
}

/* return array of node ids in graph. */
/* array of node ids should be dynamically allocated */
/* set count to be the number of nodes in graph */
/* return NULL if no vertices in graph */
int *get_vertices(Graph *graph, int *count)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to get vertices\n");
        return NULL;
    }
    return NULL;
}

/* return array of pointers to edges for a given vertex. */
/* array of edges should be dynamically allocated */
/* set count to be number of edges of vertex */
/* return NULL if no edges from/to vertex */
Edge **get_edges(Graph *graph, Vertex *vertex, int *count)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to get edges\n");
        return NULL;
    }
    return NULL;
}

/* return pointer to edge from vertex with id from, to vertex with id to. */
/* return NULL if no edge */
Edge *get_edge(Graph *graph, int from, int to)
{
    if(graph==NULL)
    {
        fprintf(stderr,"warning: Unable to get edge\n");
        return NULL;
    }
    return NULL;
}

/* return id of destination node of edge. */
int edge_destination(Edge *edge)
{
    return 0;
}

/* return weight of edge. */
double edge_weight(Edge *edge)
{
    return 0;
}

