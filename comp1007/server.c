/*
 *  NetworkServer.c
 *  ProgrammingPortfolio Skeleton
 *
 */

/* You will need to include these header files to be able to implement the TCP/IP functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "graph.h"
#include "dijkstra.h"

/* You will also need to add #include for your graph library header files */
const char *greeting="+OK 2022 Programming Portfolio Route Server by Maxim Carr\r\n";
void readCommand(int,char*,char*,char*,char*,int);
int getArgInt(char*);
int main(int argc, const char * argv[])
{
	int serverSocket = -1;
	int providedPort = 0,incoming =0,vertexID,i,j,numOfNodes,pcw;
	double k;
	struct sockaddr_in cli;
	struct sockaddr_in srv;
	Graph *dstraGraph;
	char *argptr;
	int l=0;
	printf("Programming Portfolio 2022 Implementation\n");
	printf("=========================================\n\n");
	/* Insert your code here to create, and the TCP/IP socket for the Network Server
	 *
	 * Then once a connection has been accepted implement protocol described in the coursework description.
	 */
	providedPort=strtol(argv[1],&argptr,10);
	serverSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serverSocket==-1)
	{
		fprintf(stderr,"Failure to create socket\n");
		exit(-1);
	}
	memset(&srv,0,sizeof(srv));
	srv.sin_family=AF_INET;
	srv.sin_port=htons(providedPort);
	srv.sin_addr.s_addr=INADDR_ANY;
	bind(serverSocket,(struct sockaddr*)&srv,sizeof(srv));
	listen(serverSocket,32);
	dstraGraph=init_graph();
	numOfNodes=0;
	while(1)/*while true - should mean that the stored graph will be stored fine between connections - remove if this breaks pipeline*/
	{
		char outBuffer[512], inBuffer[512], splittable[512], andagain[512],hopper[512];
		int closeFlag =0;/*0 to keep it open, 1 to close connect*/
		l=sizeof(cli);
		incoming =accept(serverSocket,(struct sockaddr*)&cli,&l);
		
		/*greeting client*/
		sprintf(outBuffer,greeting);
		write(incoming,outBuffer,strlen(outBuffer));
		
		while(closeFlag!=1)
		{
			char *cmd,*addptr,*addptr2,*addptr1;
			int ec;
			/*Command options*/
			memset(inBuffer,0,strlen(inBuffer));
			readCommand(incoming,inBuffer,splittable,andagain,hopper,512);
			if(strstr(inBuffer,"QUIT")!=NULL)
			{
				sprintf(outBuffer,"+OK\r\n");
				write(incoming,outBuffer,strlen(outBuffer));
				closeFlag=1;
			}
			else if(strstr(inBuffer,"NET-")!=NULL)/*Net Commands Subsection*/
			{
				pcw=0;
				if(strstr(inBuffer,"ADD")!=NULL)
				{
					vertexID=getArgInt(inBuffer);
					if(find_vertex(dstraGraph,vertexID)!=NULL)
					{
						sprintf(outBuffer,"-ERR Vertex already exists\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
					else
					{
						add_vertex(dstraGraph,vertexID);
						sprintf(outBuffer,"+OK Added %d\r\n",vertexID);
						write(incoming,outBuffer,strlen(outBuffer));
						numOfNodes++;
					}
				}
				else if(strstr(inBuffer,"DELETE")!=NULL)
				{
					vertexID=getArgInt(inBuffer);
					if(find_vertex(dstraGraph,vertexID)!=NULL)
					{
						remove_vertex(dstraGraph,vertexID);
						sprintf(outBuffer,"+OK Deleted %d\r\n",vertexID);
						write(incoming,outBuffer,strlen(outBuffer));
						numOfNodes--;
					}
					else
					{
						sprintf(outBuffer,"-ERR Vertex does not exist\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
				}
				else if(strstr(inBuffer,"LIST")!=NULL)
				{
					sprintf(outBuffer,"+OK %d\r\n",numOfNodes);
					write(incoming,outBuffer,strlen(outBuffer));
					for(i=0;i<32;i++)
					{
						if(find_vertex(dstraGraph,i)!=NULL)
						{
							sprintf(outBuffer,"%d\r\n",i);
							write(incoming,outBuffer,strlen(outBuffer));
						}
					}
				}
				else
				{
					sprintf(outBuffer,"-ERR Invalid command\r\n");
					write(incoming,outBuffer,strlen(outBuffer));
				}
			}
			else if(strstr(inBuffer,"ROUTE-")!=NULL)/*Route commands subsection*/
			{
				if(strstr(inBuffer,"ADD")!=NULL)
				{
					pcw=1;
					cmd=strchr(splittable,32);/*Splitting off non-int command*/
					i=strtol((strtok(cmd," ")),&addptr,10);
					j=strtol((strtok(NULL," ")),&addptr1,10);
					k=strtod((strtok(NULL," ")),&addptr2);
					if((find_vertex(dstraGraph,i)!=NULL)&&(find_vertex(dstraGraph,j)!=NULL))
					{
						if(get_edge(dstraGraph,i,j)!=NULL)
						{
							remove_edge(dstraGraph,i,j);
							remove_edge(dstraGraph,j,i);
							add_edge_undirected(dstraGraph,i,j,k);
						}
						else
							add_edge_undirected(dstraGraph,i,j,k);
						sprintf(outBuffer,"+OK Route Added\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
					else
					{
						sprintf(outBuffer,"-ERR Vertex does not exist\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
				}
				else if(strstr(inBuffer,"DELETE")!=NULL)
				{
					cmd=strchr(andagain,32);
					i=strtol((strtok(cmd," ")),&addptr,10);
					j=strtol((strtok(NULL," ")),&addptr1,10);
					if(get_edge(dstraGraph,i,j)!=NULL)
					{
						remove_edge(dstraGraph,i,j);
						remove_edge(dstraGraph,j,i);
						sprintf(outBuffer,"+OK Route Removed\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
					else
					{
						sprintf(outBuffer,"-ERR Edge does not exist\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
				}
				else if(strstr(inBuffer,"SHOW")!=NULL)/*to be implemented*/
				{
					int count=0;
					int f;
					int sourceV=getArgInt(inBuffer);
					Vertex *sv;
					Edge **arrEdge;
					Edge *cEdge;
					if(find_vertex(dstraGraph,sourceV)!=NULL)
					{
						sv=find_vertex(dstraGraph,sourceV);
						arrEdge=get_edges(dstraGraph,sv,&count);
						sprintf(outBuffer,"+OK %d\r\n",count);
						write(incoming,outBuffer,strlen(outBuffer));
						for(f=0;f<count;f++)
						{
							cEdge=arrEdge[f];
							sprintf(outBuffer,"%d\r\n",cEdge->vertex->id);
							write(incoming,outBuffer,strlen(outBuffer));
						}
					}
					else
					{
						sprintf(outBuffer,"-ERR Vertex does not exist\r\n",sourceV);
						write(incoming,outBuffer,strlen(outBuffer));
					}
				}
				else if(strstr(inBuffer,"HOP")!=NULL)
				{
					Path *hopPath;
					int next,entryCount;
					cmd=strchr(hopper,32);
					i=strtol((strtok(cmd," ")),&addptr,10);
					j=strtol((strtok(NULL," ")),&addptr1,10);
					if((find_vertex(dstraGraph,i)==NULL)||(find_vertex(dstraGraph,j)==NULL)||(i==j))
					{
						sprintf(outBuffer,"-ERR Vertices do not exist or are equal\r\n");
						write(incoming,outBuffer,strlen(outBuffer));
					}
					else
					{
						hopPath=dijkstra(dstraGraph,i,&entryCount);
						next=hopPath[j].next_hop;
						sprintf(outBuffer,"+OK %d\r\n",next);
						write(incoming,outBuffer,strlen(outBuffer));
					}
				}
				else if(strstr(inBuffer,"TABLE")!=NULL)
				{
					Path *rtTable;
					int cVertex,count,x,nextID,tc=0;
					int wght;
					cVertex=getArgInt(inBuffer);
					rtTable=dijkstra(dstraGraph,cVertex,&count);
					for(x=0;x<count;x++)
					{
						if((cVertex!=x)&&(find_vertex(dstraGraph,x)!=NULL))
							tc++;
					}
					sprintf(outBuffer,"+OK %d\r\n",tc);
					write(incoming,outBuffer,strlen(outBuffer));
					for(x=0;x<count;x++)
					{
						if((cVertex!=x)&&(find_vertex(dstraGraph,x)!=NULL)){
							wght=rtTable[x].weight;
							sprintf(outBuffer,"%d -> %d, next-hop %d, weight %d\r\n",cVertex,x,rtTable[x].next_hop,wght);
							write(incoming,outBuffer,strlen(outBuffer));
						}
					}
				}
				else
				{
					sprintf(outBuffer,"-ERR Invalid command\r\n");
					write(incoming,outBuffer,strlen(outBuffer));
				}
			}
			else
			{
				sprintf(outBuffer,"-ERR Invalid command\r\n");
				write(incoming,outBuffer,strlen(outBuffer));
			}
		}
		close(incoming);
	}
	free_graph(dstraGraph);
	return 0;
}
int getArgInt(char*input)/*works for one int commands*/
{
	char a = 32;
	char *num,*argint;
	int out;
	num=strchr(input,a);
	out=strtol(num,&argint,10);
	return out;
}
void readCommand(int incoming, char *buffer, char*splittable,char*andagain,char*hopper, int size)
{
	int i;
	read(incoming,buffer,512);
	for(i=0;i<strlen(buffer);i++)
	{
		splittable[i]=buffer[i];
		andagain[i]=buffer[i];
		hopper[i]=buffer[i];
	}
}