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

#define M_SIZE 50

// Structures
typedef struct {
	//int atime;		// arrival time
	int priority;		// priority number 1 - 5
	char id[25];			// patient id
	int Hour, minute;	// patient arrival time
	int placement;		// placement to compete with priority
	char day[3];


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
void ReadFile(HEAP* heap);
void RetrievePat(HEAP* heap);
char Menu(void);
// Heap Functions
HEAP*	CreateHeap(int maxsize);
bool 	InsertHeap(HEAP* heap, char *id, int priority, int hour, int minute, char * time, int placement);
//void 	DeleteHeap(HEAP* heap, PATIENT * pat); // retrieve data from heap
//int		HeapCount(HEAP* heap);
//bool	HeapFull(HEAP* heap);
//bool	HeapEmpty(HEAP* heap);
void	HeapSort(HEAP* heap, int last);
void	DestroyHeap(HEAP* heap);
void 	ReheapUp(HEAP* heap, int childloc);
void ReheapDown(HEAP* heap, int root, int last);
void	PrintList(HEAP* heap);

// Main
int main(void) {

	bool done = false;
	char test;
	HEAP *heap;

	heap = CreateHeap(M_SIZE);
	while (!done)
	{
		test = Menu();

		// Switch case to enter menu functions
		switch (test)
		{


		case('s'):
			// enter print list function
			ReadFile(heap);
			fflush(stdout);
			HeapSort(heap, heap->last);
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
	printf(" s 	:		read in and sort list of patients			\n");
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
		case 's':
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
void ReadFile(HEAP* heap)
{
	FILE *fp;
	int done = 0;
	int crit, hour, minute;		//
	char id[25];				// Patient ID
	int error;					// End of File
	char time[3];				// AM or PM (+ 12)
	int placement[5] = { 1, 1, 1, 1, 1 };	//
											// File I/O
	fp = fopen("test.txt", "r");
	if (!fp)
	{
		printf("File cannot be opened \n");
		fflush(stdout);
		return;
	}

	// while loop that constructs the graph relationships
	while (!done)
	{
		// Patient ID, Criticallity, Time
		error = fscanf(fp, "%[^,], %d, %d:%2d%s \n \n", id, &crit, &hour, &minute, time);
		fflush(stdin);

		// exit condition
		if (error == EOF)
		{
			done = 1; break;
		}

		// insert attributes into heap. Compare criticality and placement
		InsertHeap(heap, id, crit, hour, minute, time, placement[crit - 1]++);

		// Print the listed patients
		printf("%s, %d, %d:%d, %s  -- placement : %d \n", id, crit, hour, minute, time, placement[crit - 1] - 1);
		fflush(stdout);

	}// while loop end

	fclose(fp);
	// end File I/O
	return;
}

/* Retrieve Patient
Input: Heap
Ouptut: Retrieve root patient and patient data from heap
*/
void RetrievePat(HEAP* heap)
{
	/*bool error;
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
	*/
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

	heap->last = - 1;
	// Heap array stuff
	heap->maxsize = (int)pow(2, ceil(log2(maxsize))) - 1;
	heap->size = 0;
	//printf("%d", heap->maxsize);
	heap->heaparr = (PATIENT*)malloc(heap->maxsize * sizeof(PATIENT));

	return heap;
}

void	HeapSort(HEAP* heap, int last)
{
	// local def
	int sorted;
	int holdprior, holdhour, holdminute, holdplacement;
	char holdid[25];
	char time[3];


	printf("Starting \n");
	fflush(stdout);
	sorted = last;
	while (sorted > 0)
	{
		// transfer 1st to the hold
		holdprior = heap->heaparr[0].priority;
		holdhour = heap->heaparr[0].Hour;
		holdminute = heap->heaparr[0].minute;
		holdplacement = heap->heaparr[0].placement;
		strcpy(holdid, heap->heaparr[0].id);
		strcpy(time, heap->heaparr[0].day);

		// transfer last of the list to the root
		heap->heaparr[0].priority = heap->heaparr[sorted].priority;
		heap->heaparr[0].Hour = heap->heaparr[sorted].Hour;
		heap->heaparr[0].minute = heap->heaparr[sorted].minute;
		heap->heaparr[0].placement = heap->heaparr[sorted].placement;
		strcpy(heap->heaparr[0].id, heap->heaparr[sorted].id);
		strcpy(heap->heaparr[0].day, heap->heaparr[sorted].day);

		heap->heaparr[sorted].priority = holdprior;
		heap->heaparr[sorted].Hour = holdhour;
		heap->heaparr[sorted].minute = holdminute;
		heap->heaparr[sorted].placement = holdplacement;
		strcpy(heap->heaparr[sorted].id, holdid);
		strcpy(heap->heaparr[sorted].day, time);

		// Display the sorted value's properties
		printf("Priority: %d, ID: %s, Time: %d:%d%s Placement: %d\n", holdprior, holdid, holdhour, holdminute, time, holdplacement);
		fflush(stdout);

		sorted--;
		ReheapDown(heap, 0, sorted);

	}
	printf("Priority: %d, ID: %s, Time: %d:%d%s \n", heap->heaparr[0].priority, heap->heaparr[0].id,
		heap->heaparr[0].Hour, heap->heaparr[0].minute, heap->heaparr[0].day);
}

/* Insert Heap
* Insert new element into heap
* Input: heap data and heap
* Output: heap with new element
*/
bool InsertHeap(HEAP* heap, char *id, int priority, int hour, int minute, char * time, int placement)
{
	// empty heap
	if (heap->size == 0)
	{
		heap->size = 1;
		heap->last = 0;
		strcpy(heap->heaparr[0].id, id);
		heap->heaparr[0].priority = priority;
		heap->heaparr[0].placement = placement;
		strcpy(heap->heaparr[0].day, time);
		heap->heaparr[0].Hour = hour;
		heap->heaparr[0].minute = minute;

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
	heap->heaparr[heap->last].placement = placement;
	strcpy(heap->heaparr[heap->last].day, time);
	heap->heaparr[heap->last].Hour = hour;
	heap->heaparr[heap->last].minute = minute;
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
	int holdprior, holdhour, holdminute, holdplacement;
	char holdid[25];
	char time[3];

	// if not at root of heap == index 0
	if (childloc)
	{
		heaparr = heap->heaparr;
		parent = (childloc - 1) / 2;	// since tree is binary child is parent - 1 / 2
										// if child is greater than the parent
		if (ComparePat(heap->heaparr[childloc].priority,
			heap->heaparr[parent].priority) == 1)
		{
			// hold the parent's value
			holdprior = heaparr[parent].priority;
			holdhour = heaparr[parent].Hour;
			holdminute = heaparr[parent].minute;
			holdplacement = heaparr[parent].placement;
			strcpy(time, heaparr[parent].day);
			strcpy(holdid, heaparr[parent].id);
			// transfer child to parent
			heaparr[parent].priority = heaparr[childloc].priority;
			heaparr[parent].Hour = heaparr[childloc].Hour;
			heaparr[parent].minute = heaparr[childloc].minute;
			heaparr[parent].placement = heaparr[childloc].placement;
			strcpy(heaparr[parent].id, heaparr[childloc].id);
			strcpy(heaparr[parent].day, heaparr[childloc].day);
			// transfer parent data to child
			heaparr[childloc].priority = holdprior;
			heaparr[childloc].Hour = holdhour;
			heaparr[childloc].minute = holdminute;
			heaparr[childloc].placement = holdplacement;
			strcpy(heaparr[childloc].id, holdid);
			strcpy(heaparr[childloc].day, time);
			// call itself again
			ReheapUp(heap, parent);
		} // end if
		else if ((ComparePat(heap->heaparr[childloc].placement, heap->heaparr[parent].placement))
			&& (ComparePat(heap->heaparr[childloc].priority, heap->heaparr[parent].priority) == -1))
		{
			// hold the parent's value
			holdprior = heaparr[parent].priority;
			holdhour = heaparr[parent].Hour;
			holdminute = heaparr[parent].minute;
			holdplacement = heaparr[parent].placement;
			strcpy(time, heaparr[parent].day);
			strcpy(holdid, heaparr[parent].id);
			// transfer child to parent
			heaparr[parent].priority = heaparr[childloc].priority;
			heaparr[parent].Hour = heaparr[childloc].Hour;
			heaparr[parent].minute = heaparr[childloc].minute;
			heaparr[parent].placement = heaparr[childloc].placement;
			strcpy(heaparr[parent].id, heaparr[childloc].id);
			strcpy(heaparr[parent].day, heaparr[childloc].day);
			// transfer parent data to child
			heaparr[childloc].priority = holdprior;
			heaparr[childloc].Hour = holdhour;
			heaparr[childloc].minute = holdminute;
			heaparr[childloc].placement = holdplacement;
			strcpy(heaparr[childloc].id, holdid);
			strcpy(heaparr[childloc].day, time);
			// call itself again
			ReheapUp(heap, parent);

		}

	} // end if
	return;
}

/* Delete Heap
Input: Heap and structure passed by reference
Output: boolean value to indicate if deletetion was a success
*/
/*void DeleteHeap(HEAP* heap, PATIENT * pat)
{
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
*/
/* Reheap Down
Input: Heap and root
Ouput: N/A
start from top and see if root is greater than chilren or replace with largest child
*/
void ReheapDown(HEAP* heap, int root, int last)
{
	// local def
	//int last;
	int prior_l, prior_r;		// left and right child priority
	int largeloc;
	// data holding
	int holdprior, holdhour, holdminute, holdplacement;
	char holdid[25];
	char time[3];

	//
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
		else if ((ComparePat(prior_l, prior_r) == -1) && (ComparePat(heap->heaparr[root * 2 + 1].placement, heap->heaparr[root * 2 + 2].placement)))
		{
			largeloc = root * 2 + 1;
		} // end if
		else
		{
			largeloc = root * 2 + 2;
		}
		if (ComparePat(heap->heaparr[largeloc].priority,
			heap->heaparr[root].priority) == 1)
		{

			// hold the parent's value
			holdprior = heap->heaparr[root].priority;
			holdhour = heap->heaparr[root].Hour;
			holdminute = heap->heaparr[root].minute;
			holdplacement = heap->heaparr[root].placement;
			strcpy(time, heap->heaparr[root].day);
			strcpy(holdid, heap->heaparr[root].id);
			// transfer child to parent
			heap->heaparr[root].priority = heap->heaparr[largeloc].priority;
			heap->heaparr[root].Hour = heap->heaparr[largeloc].Hour;
			heap->heaparr[root].minute = heap->heaparr[largeloc].minute;
			heap->heaparr[root].placement = heap->heaparr[largeloc].placement;
			strcpy(heap->heaparr[root].id, heap->heaparr[largeloc].id);
			strcpy(heap->heaparr[root].day, heap->heaparr[largeloc].day);
			// transfer parent data to child
			heap->heaparr[largeloc].priority = holdprior;
			heap->heaparr[largeloc].Hour = holdhour;
			heap->heaparr[largeloc].minute = holdminute;
			heap->heaparr[largeloc].placement = holdplacement;
			strcpy(heap->heaparr[largeloc].id, holdid);
			strcpy(heap->heaparr[largeloc].day, time);
			// call itself again
			ReheapDown(heap, largeloc, last);


		} // end if
		else if ((ComparePat(heap->heaparr[largeloc].placement, heap->heaparr[root].placement))
			&& (ComparePat(heap->heaparr[largeloc].priority, heap->heaparr[root].priority) == -1))
		{
			// hold the parent's value
			holdprior = heap->heaparr[root].priority;
			holdhour = heap->heaparr[root].Hour;
			holdminute = heap->heaparr[root].minute;
			holdplacement = heap->heaparr[root].placement;
			strcpy(time, heap->heaparr[root].day);
			strcpy(holdid, heap->heaparr[root].id);
			// transfer child to parent
			heap->heaparr[root].priority = heap->heaparr[largeloc].priority;
			heap->heaparr[root].Hour = heap->heaparr[largeloc].Hour;
			heap->heaparr[root].minute = heap->heaparr[largeloc].minute;
			heap->heaparr[root].placement = heap->heaparr[largeloc].placement;
			strcpy(heap->heaparr[root].id, heap->heaparr[largeloc].id);
			strcpy(heap->heaparr[root].day, heap->heaparr[largeloc].day);
			// transfer parent data to child
			heap->heaparr[largeloc].priority = holdprior;
			heap->heaparr[largeloc].Hour = holdhour;
			heap->heaparr[largeloc].minute = holdminute;
			heap->heaparr[largeloc].placement = holdplacement;
			strcpy(heap->heaparr[largeloc].id, holdid);
			strcpy(heap->heaparr[largeloc].day, time);
			// call itself again
			ReheapDown(heap, largeloc, last);
		}
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
	if (heap != NULL)
	{
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
	if (prior1 < prior2)
		return 1;
	if (prior1 == prior2)
		return -1;
	else
		return 0;
}

void PrintList(HEAP *heap)
{
	int i;
	PATIENT *pat;
	if (heap->size != 0)
	{
		pat = heap->heaparr;
		for (i = 0; i < heap->size; i++)
		{
			printf("Pateint: ID %s , Criticalility %d ", pat->id, pat->priority);
			pat++;

		}
	}
	else
	{
		printf("no patients \n");
	}


}
