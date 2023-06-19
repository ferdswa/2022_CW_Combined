/*
 *  dijkstra.c
 *  ProgrammingPortfolio
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"


/* find shortest paths between source node id and all other nodes in graph. */
/* upon success, returns an array containing a table of shortest paths.  */
/* return NULL if *graph is uninitialised or an error occurs. */
/* each entry of the table array should be a Path */
/* structure containing the path information for the shortest path between */
/* the source node and every node in the graph. If no path exists to a */
/* particular desination node, then next should be set to -1 and weight */
/* to DBL_MAX in the Path structure for this node */
Path *dijkstra(Graph *graph, int id, int *pnEntries)
{
    Path *table=NULL;
    Edge *cEdge;
    int *setOfAllNodes,i,sizeOfSetS;
    int vertexCount=0;
    int closestNeighbor;
    /* Insert your implementation of Dijkstra's algorithm here */
    if(graph==NULL)
        return NULL;
    else if(find_vertex(graph,id)==NULL)
        return NULL;
    setOfAllNodes=get_vertices(graph,&vertexCount);
    table=calloc(vertexCount+1,sizeof(Path));
    for(i=0;i<vertexCount;i++)/*Removing source node - smallSet is set S*/
    {
        if(setOfAllNodes[i]==id)
            i++;
    }
    for(i=0;i<vertexCount;i++)/*Creating R and D tables*/
    {
        cEdge=get_edge(graph,id,setOfAllNodes[i]);
        table[setOfAllNodes[i]].weight=DBL_MAX;
        table[setOfAllNodes[i]].next_hop=0;
        if(cEdge!=NULL)
        {
            table[setOfAllNodes[i]].next_hop=cEdge->vertex->id;
            table[setOfAllNodes[i]].weight=cEdge->weight;
        }
    }
    sizeOfSetS=vertexCount;
    
    while(sizeOfSetS>0)
    {
        Edge **edgeA;
        int u,j,found=0,neighborCount;
        double c;
        closestNeighbor=setOfAllNodes[0];
        for(i=0;i<sizeOfSetS;i++)
        {
            if(table[closestNeighbor].weight>table[setOfAllNodes[i]].weight)
                closestNeighbor=setOfAllNodes[i];
        }
        u=closestNeighbor;
        i=0;
        while(found==0)
        {
            if(setOfAllNodes[i]==u)
                found=1;
            else
                i++;
        }
        for(j=i;j<sizeOfSetS;j++)
        {
            if(j<sizeOfSetS-1)
                setOfAllNodes[j]=setOfAllNodes[j+1];
        }
        if(u==0)
        {
            sizeOfSetS=0;
        }
        else
        {
            edgeA=get_edges(graph,find_vertex(graph,u),&neighborCount);
            for(i=0;i<neighborCount;i++)
            {
                int isInSetS = 0;
                Vertex *currentVertex = edgeA[i]->vertex;
                for(j=0;j<sizeOfSetS;j++)
                {
                    if(currentVertex->id==setOfAllNodes[j])
                        isInSetS=1;
                }
                if(isInSetS==1)
                {
                    c=table[u].weight+edge_weight(edgeA[i]);
                    if(c<table[currentVertex->id].weight)
                    {
                        table[currentVertex->id].weight=c;
                        table[currentVertex->id].next_hop=table[u].next_hop;
                    }
                }
            }
            if(edgeA!=NULL)
                free(edgeA);
        }
        sizeOfSetS--;
    }
    table[id].next_hop=-1;
    table[id].weight=DBL_MAX;
    *pnEntries=vertexCount+1;
    free(setOfAllNodes);
    return table;
}