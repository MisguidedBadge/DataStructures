/*
============================================================================
Name        : HosptialTrey.c
Author      : Trey
Version     :
Copyright   : Yeet
Description : Hello World in C, Ansi-style
============================================================================
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define M_SIZE 50

// Structures
typedef struct {
	struct tm atime;		// arrival time
	int priority;	// priority number 1 - 5
	char id[25];		// patient id


} PATIENT;

typedef struct {
	PATIENT * heaparr;
	int last;		// last element
	int size;		// current size
	int maxsize;	// max size of heap
}HEAP;
//

// Functions Prototypes
int ComparePat(int prior1, int prior2);
void EnterPat(HEAP* heap);
void RetrievePat(HEAP* heap);
char Menu(void);
// Heap Functions
HEAP*	CreateHeap(int maxsize);
bool 	InsertHeap(HEAP* heap, char *id, int priority, struct tm atimer);
void 	DeleteHeap(HEAP* heap, PATIENT * pat); // retrieve data from heap
//int		HeapCount(HEAP* heap);
//bool	HeapFull(HEAP* heap);
//bool	HeapEmpty(HEAP* heap);
void	DestroyHeap(HEAP* heap);
void 	ReheapUp(HEAP* heap, int childloc);
void	ReheapDown(HEAP* heap, int root);
void	PrintList(HEAP* heap);

// GLOBAL VARIABLES
// TIME STUFF


// Main
int main(void) {

	bool done = false;
	char input;
	HEAP *heap;

	// Create the heap
	heap = CreateHeap(M_SIZE);

	// Start timer for to fill in time for patients

	while (!done)
	{
		input = Menu();

		// Switch case to enter menu functions
		switch (input)
		{
		case('i'):
			// enter the insert function
			EnterPat(heap);
			break;
		case('r'):
			// enter the retrieve function
			RetrievePat(heap);
			break;
		case('l'):
			// enter print list function
			PrintList(heap);
			break;
		case('e'):
			// exit main program
			done = true;
			break;
		default:
			// bad input made it through
			printf("Bad input got through error \n");
			//return 1;
			break;
		} // END SWITCH

	}

	return EXIT_SUCCESS;
}

// Menu
/* Program menu: Insert, Retrieve, Print
* Input: nothing
* Output: Character (menu option)
*/
char Menu(void)
{
	char input;
	bool error = true;
	// Display menu items
	printf("\n =============== Menu =======================		\n");
	printf(" i	:		Enter patient into system		\n");
	printf(" r	:		Retrieve patient from system	\n");
	printf(" l 	:		Print list of patients			\n");
	printf(" e 	:		Exit program					\n");
	// Flush standard output
	fflush(stdout);

	while (error)
	{
		// scan input
		scanf("%c", &input);
		fflush(stdin);
		// change case to lower
		input = tolower(input);
		switch (input)
		{
		case 'i':		// all valid input options
		case 'r':
		case 'l':
		case 'e':
			error = false;
			break;
		default: printf("Enter again: ");
			fflush(stdout);
			break;
		} // END SWITCH

	} // END While

	return input;
}



/* Enter Patient
	Input: Heap
	Output: inserted patient into heap
*/
void EnterPat(HEAP* heap)
{
	bool error = 1;
	char id[25];
	int priority;
	time_t p_time;
	struct tm * pt;

	// prompt for user ID and priority
	printf("enter patient id and Criticality  \n");
	fflush(stdout);
	//fgets(id, 5, stdin);
	scanf("%s", id);
	fflush(stdin);
	scanf("%d", &priority);
	fflush(stdin);

	// pateint time
	time(&p_time);
	pt = localtime(&p_time);
	printf("Time entered Day: %d, Hour: %d, Minutes: %d, Seconds: %d \n", pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);


	// insert function
	error = InsertHeap(heap, id, priority, *pt);

	// check if heap is too big to insert
	if (error)
	{
		printf("Heap too big to insert \n");
	}
	return;
}

/* Retrieve Patient
	Input: Heap
	Ouptut: Retrieve root patient and patient data from heap
	*/
void RetrievePat(HEAP* heap)
{
	PATIENT *pat;

	if (heap->size == 0)
	{
		// no patients then exit
		printf("No patients in database to be retrieved \n");
	}

	else
	{
		// Retrieve patient from the top (highest priority)
		pat = heap->heaparr;
		DeleteHeap(heap, pat);
	}

}

/* Create Heap
*	Input: Heap pointer
*	Output: Created Heap
*/

HEAP* CreateHeap(int maxsize)
{
	HEAP* heap;

	// allocate memory for heap
	heap = (HEAP*)malloc(sizeof(HEAP));
	if (!heap)
	{
		printf("Not enough memory for heap \n");
		return NULL;
	}

	heap->last = -1;
	// Heap array stuff
	heap->maxsize = (int)pow(2, ceil(log2(maxsize))) - 1;
	heap->size = 0;
	//printf("%d", heap->maxsize);
	heap->heaparr = (PATIENT*)malloc(heap->maxsize * sizeof(PATIENT));

	return heap;
}

/* Insert Heap
* Insert new element into heap
* Input: heap data and heap
* Output: heap with new element
*/
bool InsertHeap(HEAP* heap, char *id, int priority, struct tm atimer)
{
	// empty heap set values for that
	if (heap->size == 0)
	{
		heap->size = 1;
		heap->last = 0;
		strcpy(heap->heaparr[0].id, id);
		heap->heaparr[0].priority = priority;
		// assign time
		heap->heaparr[0].atime.tm_mday = atimer.tm_mday;
		heap->heaparr[0].atime.tm_hour = atimer.tm_hour;
		heap->heaparr[0].atime.tm_min = atimer.tm_min;
		heap->heaparr[0].atime.tm_sec = atimer.tm_sec;
		return false;
	} // end if

	  // heap too big
	if (heap->last == heap->maxsize - 1)
		return true;

	// increment heap size and insert data
	(heap->last)++;
	(heap->size)++;
	strcpy(heap->heaparr[heap->last].id, id);
	heap->heaparr[heap->last].priority = priority;
	// assign time
	heap->heaparr[heap->last].atime.tm_mday = atimer.tm_mday;
	heap->heaparr[heap->last].atime.tm_hour = atimer.tm_hour;
	heap->heaparr[heap->last].atime.tm_min = atimer.tm_min;
	heap->heaparr[heap->last].atime.tm_sec = atimer.tm_sec;
	ReheapUp(heap, heap->last);			// heap up to see if priority need to be lifted

	return false;
}

/* ReheapUp
* Moves child up if larger. This occurs after insertion
* Input: Heap and child location
* Ouptut: N/A
*/
void ReheapUp(HEAP * heap, int childloc)
{
	// local def
	int parent;
	PATIENT * heaparr;
	int holdprior;
	char holdid[6];

	// if not at root of heap == index 0
	if (childloc)
	{
		heaparr = heap->heaparr;
		parent = (childloc - 1) / 2;	// since tree is binary child is parent - 1 / 2
										// if child is greater than the parent
		if (ComparePat(heap->heaparr[childloc].priority,
			heap->heaparr[parent].priority))
		{
			// hold the parent's value
			holdprior = heaparr[parent].priority;
			//holdtime	= heaparr[parent].atime;
			strcpy(holdid, heaparr[parent].id);
			// transfer child to parent
			heaparr[parent].priority = heaparr[childloc].priority;
			heaparr[parent].atime = heaparr[childloc].atime;
			strcpy(heaparr[parent].id, heaparr[childloc].id);
			// transfer parent data to child
			heaparr[childloc].priority = holdprior;
			//heaparr[childloc].atime = holdtime;
			strcpy(heaparr[childloc].id, holdid);
			// call itself again
			ReheapUp(heap, parent);
		} // end if
	} // end if
	return;
}

/* Delete Heap
	Input: Heap and structure passed by reference
	Output: boolean value to indicate if deletetion was a success
*/
void DeleteHeap(HEAP* heap, PATIENT * pat)
{
	printf("Processing Patient: %s, Criticality : %d, Time entered Day: %d, Hour: %d, Minutes: %d, Seconds: %d",
			pat->id, pat->priority, pat->atime.tm_mday, pat->atime.tm_hour, pat->atime.tm_min, pat->atime.tm_sec);
	// transfer data to go out
	pat->priority = heap->heaparr[0].priority;
	pat->atime = heap->heaparr[0].atime;
	strcpy(pat->id, heap->heaparr[0].id);

	// transfer last item to go into root and then reheap down
	heap->heaparr[0].priority = heap->heaparr[heap->last].priority;
	heap->heaparr[0].atime = heap->heaparr[heap->last].atime;
	strcpy(heap->heaparr[0].id, heap->heaparr[heap->last].id);

	// decrement heap array then reheap down
	(heap->last)--;
	(heap->size)--;
	ReheapDown(heap, 0);

	return;
}

/* Reheap Down
	Input: Heap and root
	Ouput: N/A
	start from top and see if root is greater than chilren or replace with largest child
*/
void ReheapDown(HEAP* heap, int root)
{
	// local def
	int last;
	int prior_l, prior_r;		// left and right child priority
	int largeloc;
	// data holding
	int holdprior;
	int holdtime;
	char holdid[6];

	//
	last = heap->last;
	if ((root * 2 + 1) <= last)	// see if there are children
	{
		prior_l = heap->heaparr[root * 2 + 1].priority;
		if ((root * 2 + 2) <= last) // see if right exists
		{
			prior_r = heap->heaparr[root * 2 + 2].priority;
		}
		else
		{
			prior_r = 0;			// doesnt exist
		} // end if

		if ((!prior_r) || ComparePat(prior_l, prior_r) == 1)
		{
			largeloc = root * 2 + 1;
		}
		else
		{
			largeloc = root * 2 + 2;
		} // end if
		if (ComparePat(heap->heaparr[largeloc].priority,
			heap->heaparr[root].priority) == 1)
		{
			// child > parent
			// hold the parent's value
			holdprior = heap->heaparr[root].priority;
			//holdtime = heap->heaparr[root].atime;
			strcpy(holdid, heap->heaparr[root].id);
			// transfer child to parent
			heap->heaparr[root].priority = heap->heaparr[largeloc].priority;
			heap->heaparr[root].atime = heap->heaparr[largeloc].atime;
			strcpy(heap->heaparr[root].id, heap->heaparr[largeloc].id);
			// transfer parent data to child
			heap->heaparr[largeloc].priority = holdprior;
			//heap->heaparr[largeloc].atime = holdtime;
			strcpy(heap->heaparr[largeloc].id, holdid);
			// reheap down
			ReheapDown(heap, largeloc);
		} // end if
	}// end if
	return;
}

/* Destroy Heap
	Input: Heap
	Output: Nothing
	Destroy heap at end of program
*/
void DestroyHeap(HEAP * heap)
{
	// see if heap exists
	if (heap != NULL)
	{
		// free heap array if it exists
		if (heap->heaparr != NULL)
		{
			free(heap->heaparr);
		}
		free(heap);
	}
	return;
}

/* Compare Patient
* Input: Patient 1 & 2's Priority
* Output: Patient with greater priority (group same as less)
* return 1 if greater, return 0 if lesser or equal
*/
int ComparePat(int prior1, int prior2)
{
	// compare patient priorites
	if (prior1 < prior2)
		return 1;
	else
		return 0;
}

void PrintList(HEAP *heap)
{
	// local def
	int i;
	PATIENT *pat;

	// check if heap has anything
	if (heap->size != 0)
	{
		// go through heap array and print contents
		pat = heap->heaparr;
		for (i = 0; i < heap->size; i++)
		{
			printf("Pateint: ID %s , Criticality  %d , Time entered Day: %d, Hour: %d, Minutes: %d, Seconds: %d \n"
				, pat->id, pat->priority, pat->atime.tm_mday, pat->atime.tm_hour, pat->atime.tm_min, pat->atime.tm_sec);
			//printf("Pateint: ID %s , Criticalility %d , Time entered Day: %d"
			//					, pat->id, pat->priority, pat->atime->tm_mday);
			pat++;

		}
	}
	else
	{
		printf("no patients \n");
	}


}
