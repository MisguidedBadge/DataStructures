/*
 ============================================================================
 Name        : nqueenstp.c
 Author      : Trey
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

// Structures
struct q_node {
	//Queen Attributes
	int data[2];	// row then column
	// pointer to node "below"
	struct q_node *link;
};
struct stack {
	int count;				// stack counter
	struct q_node *top;		// pointer that points to top of stack
};

// Prototype Functions
int 	Check_Queen(int row, int column, int * board, int size);
int* 	Create_Board(int n);
void 	Print_Board(int* board, int size);
// Prototype Stack functions
void 	Print_Queens(struct stack *stak, int count, int size);
struct  stack* New_Stack();
int 	Push_Stack(struct stack* stak, int row, int column);
int * 	Pop_Stack(struct stack* stak);
void	Delete_Stack(struct stack* stak);


// MAIN
int main(void) {

	// Local Variables
	int n;					// number of queens wanting to place
	int num_queens = 0;			// number of queens placed
	int *board, *data;
	struct stack * stak;	// board pointer
	int error = 0;			// error checker
	int row = 0;			// row and column values for board marking
	int column = 0;			//
	double timer;
	clock_t start, end;		// timer variables
	int counter = 0;		// check how many time progam goes through loop

	// START
	// Query for the number of queens and intro
	printf("Welcome to nqueens \n");
	while (!error)
	{
		printf("Enter a number of queens greater than 3: ");
		fflush(stdout);
		scanf("%d", &n);
		fflush(stdin);
		// Nqueens only works for values greater than 3
		if(n > 3)
		{
			break;
		}
	}

	// Create board of size nxn
	board = Create_Board(n);

	// Start Timer
	start = clock();
	//
	// Create Stack
	stak = New_Stack();
	// Start queens and keep going until rows are over amount
	while(row < n)
	{
		// check row and column
			if(Check_Queen(row, column, board, n) && column < n)
			{
				// assign queen in that position
				board[row * n + column] = 1;
				Push_Stack(stak, row, column);
				// Increment row and prepare column for increment to origin
				row++;
				column = -1;
				num_queens++;
				Print_Queens(stak, num_queens, n);
			}// end if
			while( column >= n)
			{
				// data output is row and column
				data = Pop_Stack(stak);
				if(data != NULL)
				{
					// Set the row and column
					row = data[0];
					column = data[1];
					board[data[0] * n + data[1]] = 0;
					// Free allocated memory for the 2 element array
					free(data);
					num_queens--;
					Print_Queens(stak, num_queens, n);
				}// end if
				else
				{
					printf("Not enough memory or no moves remaining\n");
					return 1;
				}
			} // while
			counter++;
			column++;

			//printf("column: %d \n", column);
	} // while

	Delete_Stack(stak);
	end = clock();
	timer = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time: %f Count: %d\n", timer, counter);

	Print_Board(board,n);
	printf("done \n");
	return EXIT_SUCCESS;
}

/* Check Queen
 *	Input: row, column, board
 *	Output: true or false
 *	check previous rows and same column for the same queen
 *	check the diagonals for a queen
 *	0 is guarded
 *	1 is unguarded
 */
int Check_Queen(int row, int column, int * board, int size)
{
	int i, j;
	// cycle 3 for loops

	// column
	for(i = row; i >= 0; i--)
	{
		// Check Rows
		if(board[i * size + column] != 0)
		{
			//printf("Row Hit %d: %d: %d \n", i, column, row);
			return 0;
		}
	}

	// Check to see if the left diagonal is guarded
	j = column;
	for( i = row; i >= 0; i--)
	{
		// check if the left diagonal checker is out of bounds
		if (j < 0)
			break;

		if((board[i * size + j] != 0))
		{	//printf("Left diagonal \n");
			return 0;
		}
		j--;
	}

	// Check to see if the right diagonal is guarded
	j = column;
	for( i = row; i >= 0; i--)
	{
		// Check if the right diagonal checker is out of bounds
		if(j >= size)
			break;

		if((board[i * size + j] != 0))
		{	//printf("Right diagonal \n");
			return 0;
		}
		j++;
	}
	// passes all checks
	return 1;
}


/* Create Board
 *	Input: Size nxn board
 *	Output: Size nxn board initialized to 0;
 */
int* Create_Board(int n)
{
	// declare local variables
	int i, j;

	// allocate a block of memory for 2D array
	int *board = (int*)malloc(n*n*sizeof(int));

	// Check if board allocation fails
	if(board == NULL)
	{
		printf("Board allocation failed \n");
		fflush(stdout);
		return board;
	}

	// assign the value of zero for each element
	for( i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			board[i * n + j] = 0;
	}

	// return board of size nxn;
	return board;
}

/* New Stack
 *	Input: None
 *	Output: Empty Stack
 */
struct stack* New_Stack()
{
	// Local Variables
	struct stack* stak;

	// allocate memory for stack
	stak = (struct stack*)malloc(sizeof(struct stack));
	// check if allocated memory is enough
	// if so then initialize stack properties
	if(stak)
	{
		stak->count = 0;
		stak->top = NULL;
		return stak;
	}
	// allocation failure
	else
	{
		printf("Not enough memory to allocate stack \n");
		fflush(stdout);
		return stak;
	}
}

/* Push Stack
 * 	Input: pointer to stack, row value and column value for new queen
 *	Output:Queen data inserted into stack
 */
int Push_Stack(struct stack* stak, int row, int column)
{
	// Local Variables
	struct q_node *new_node;

	// Allocate memory for new node
	new_node = (struct q_node*)malloc(sizeof(struct q_node));
	// if true then allocation failure
	if(new_node == NULL)
		return 0;

	// Assign queen properties
	new_node->data[0] = row;
	new_node->data[1] = column;
	new_node->link = stak->top;
	// Move stack pointer back to the top
	stak->top = new_node;
	stak->count++;
	return 1;
}

/* Pop Stack
 *	Input: pointer to stack
 *	Output: returns the row and column to be deleted
 */
int * Pop_Stack(struct stack* stak)
{
	// Local Variables
	int* output;
	struct q_node* temp;

	// If there isn't anything on the stack then output NULL
	if ( stak->count == 0)
		output = NULL;
	else
	{
		output = (int*)malloc(sizeof(int) * 2);
		// pop queen off of the stack and then free node from memory
		temp		= stak->top;
		output[0]	= temp->data[0];
		output[1]	= temp->data[1];
		stak->top	= temp->link;
		free(temp);
		(stak->count)--;
	}
	return output;
}

/*	Delete Stack
 *	Input: Stack
 *	Output: Deleted Stack
 */
void Delete_Stack(struct stack* stak)
{
	// Local Variables
	struct q_node * temp;

	// if stack is present
	if(stak)
	{
		// delete all nodes first
		while(stak->top != NULL)
		{
			// Delete node by popping off nodes off the stack;
			temp = stak->top;
			stak->top = temp->link;
			free(temp);
		}// WHILE
		free(stak);
	}// IF
	return;
}

/*	Print Queens
 *	Input: Stack with top pointed, and number of placed queens and board size
 *	Output: Nothing return but prints out values in stack and unplaced queens
 *
 */
void Print_Queens(struct stack *stak, int count, int size)
{
	// Local Variables
	struct q_node *temp;
	int i;

	// Print placed queens //
	temp = stak->top;
	//start from top (last to first)
	for( i = count; i > 0; i--)
	{
		printf("Queen: %d, row: %d, column: %d \n", i, temp->data[0] + 1, temp->data[1] + 1);
		fflush(stdout);
		temp = temp->link;	// last link of the chain is NULL
	}

	// Print unplaced queens //
	for(i = size; i > count; i--)
	{
		printf("Queen: %d, row: -1, column: -1 \n", i);
		fflush(stdout);
	}
	printf("End Move \n");
}

/* Print Board
 *	Input: board and size
 *	Output: printed board
 *
 */
void Print_Board(int *board, int size)
{
	// local variables
	int i, j;

	// Print the elements individually
	for( i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			printf("%d ", board[i * size + j]);
		}
		printf("\n");
	}

}
