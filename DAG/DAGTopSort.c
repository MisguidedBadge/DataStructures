/*
 ============================================================================
 Name        : DAGTopSort.c
 Author      : Trey
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define VERTICES 10

// Vertices
// Arc
typedef struct arc {
	struct vertex * destination;
	struct arc * pnextarc;
} ARC;


// Vertex data structure
typedef struct vertex {
	int 	next[VERTICES];			// matrix mapping next vertices if true 1 false 0
	char 	value;
	int 			indegree;		// input
	int 			outdegree;		// output
	short 			processed;		// if processed
	struct arc 		parc;			//
} VERTEX;

typedef struct node{
	int index;		// vertex index
	struct node * next;
} NODE;

typedef struct
{
	NODE * front;
	NODE * rear;
	int		 count;
} QUEUE;

// prototype functions
int FindVertex(VERTEX vtex[], char value, int numvert);
int Dequeue(QUEUE * queue);	// remove from front
void Enqueue(QUEUE * queue, int index);	// push into rear
void DestroyQueue(QUEUE * queue);
QUEUE * CreateQueue(void);



// MAIN //
int main(void) {
	// Local Declarations
	FILE * fp;
	VERTEX vtex[VERTICES]; // static amount of vertices
	QUEUE *queue;
	int i, j, from_index, to_index;
	int numvert = 0;		// number of vertexes used
	int newvert[2];
	char from, to;
	int done = 0;
	int processed = 0;
	int process_vertex;
	//int value;


	// Starting stuff
	queue = CreateQueue();
	// instantiate values to default
	for(i = 0; i < VERTICES; i++)
	{
		vtex[i].value = 0;
		vtex[i].indegree = 0;
		vtex[i].outdegree = 0;
		vtex[i].processed = 0;
		for(j = 0; j < VERTICES; j++)
			vtex[i].next[j] = 0;
	}

	// File I/O
	fp = fopen("test.txt", "r");
	if(!fp)
		return -1;

	// while loop that constructs the graph relationships
	while(!done)
	{
		// read character
		from = fgetc(fp);

		// exit condition
		if(from == -1)
		{ done = 1; break; }
		// From comma To
		else if ( (from >= 65 && from <= 90) || (from >= 97 && from <= 122))
		{
			// take out the comma
			to = fgetc(fp);
			// read in the to value
			to = fgetc(fp);
			//printf("%c, %c \n",from, to);
			// go through vertices
			for(i = 0; i < numvert; i++)
			{
				// from already in
				if(vtex[i].value == from)
				{
					newvert[0] = 1;
				}
				// to already in
				if(vtex[i].value == to)
				{
					newvert[1] = 1;
				}
			} // end for

			// place the new vertex from
			if(newvert[0] != 1)
			{
				// insert vertex into list
				vtex[numvert].value = from;
				// increment the number of vertices
				numvert++;
			}
			// place the new vertex to
			if(newvert[1] != 1)
			{
				// insert vertex into list
				vtex[numvert].value = to;
				// increment the number of vertices
				numvert++;
			}

			newvert[0] = 0;
			newvert[1] = 0;

			// pair the vertex relationships
			to_index = FindVertex(vtex, to, numvert);
			from_index = FindVertex(vtex, from, numvert);

			// connect vertices
			vtex[from_index].next[to_index] = 1;
			vtex[from_index].outdegree++;
			vtex[to_index].indegree++;

		}// end if vertex relationship


	}// while loop end

	fclose(fp);
	// end File I/O

	printf("Start Topical Order \n");

	// While loop to process the DAG
	while(processed < VERTICES){
		for(i = 0; i < VERTICES; i++)
		{
			// enqueue stuff with no in degree
			if((vtex[i].indegree == 0) && (vtex[i].processed == 0))
				{
				vtex[i].processed = 1;
				Enqueue(queue, i);
				}
		}

		// dequeue and then requeue it's children
		process_vertex = Dequeue(queue);

		if(process_vertex == -1)
			return 1;

		printf("%c \n", vtex[process_vertex].value);
		fflush(stdout);
		processed++;

		for(i = 0; i < VERTICES; i++)
		{
			// from -> to the to index needs to delete
			if(vtex[process_vertex].next[i] == 1)
			{
				// find the input vertex
				//to_index = FindVertex(vtex, to, numvert);
				(vtex[i].indegree--);

			}// if statement
		} // for loop

	}	// while loop

	//for(i = 0; i < VERTICES; i++)
	//	printf("%c \n", vtex[i].value);

	return EXIT_SUCCESS;
}

// Function that returns the index for the vertex of the value
int FindVertex(VERTEX vtex[], char value, int numvert)
{
	int i;

	// look for vertex value
	for(i = 0; i < numvert; i++)
	{
		// return index containing the vertex
		if(vtex[i].value == value)
			return i;
	}
	// value not found
	return -1;
}

// Create Queue
QUEUE * CreateQueue(void)
{
	// Local Definitions
	QUEUE * queue;

	queue = (QUEUE *)malloc(sizeof(QUEUE));
	// queue default values
	if(queue)
	{
		queue->front = NULL;
		queue->rear = NULL;
		queue->count = 0;
	}
	return queue;
}

// Enqueue vertex index into queue
void Enqueue(QUEUE * queue, int index)
{
	// local
	NODE *newptr;

	if(!(newptr = (NODE *)malloc(sizeof(NODE))))
		printf("Failed to allocate \n");

	// default values
	newptr->index = index;
	newptr->next = NULL;

	// if chain setting spot in queue
	if(queue->count == 0)
		queue->front = newptr;
	else
		queue->rear->next = newptr;

	(queue->count)++;
	queue->rear = newptr;

	return;
}

// Dequeue vertex
int Dequeue(QUEUE * queue)
{
	// local
	NODE * deleteloc;
	int index;

	// no index
	if(!queue->count)
		return -1;

	// index and delete location
	index = queue->front->index;
	deleteloc = queue->front;

	// rearrange pointers
	if(queue->count == 1)
		queue->rear = queue->front = NULL;
	else
		queue->front = queue->front->next;
	(queue->count)--;

	// free data
	free(deleteloc);

	return index;
}

// Destroy Queue
void DestroyQueue(QUEUE * queue)
{
	// local
	NODE * deleteptr;

	if(queue)
	{
		while(queue->front != NULL)
		{
			// delete enqued items
			free(queue->front->index);
			deleteptr = queue->front;
			queue->front = queue->front->next;
			free(deleteptr);
		} // while
		free(queue);
	} // if
	return;
}

