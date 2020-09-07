/*
 ============================================================================
 Name        : nqueenstp.c
 Author      : Trey
 Version     :
 Copyright   : Your copyright notice
 Description : Queen Tree
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"


 // Structures
struct q_node {
	//Queen Attributes
	int data[2];	// row then column
	int solution;	// solution if 1; 0 otherwise
	// pointer to node branches
	struct q_node **branch;
	// pointer to parent node
	struct q_node *parent;
};
struct TREE {
	//int num;					// numbers of leaves in the trees
	struct q_node *root;		//dummy root that has multiple children
};

// Prototype Functions
int 	Check_Queen(int row, int column, int * board, int size);
int* 	Create_Board(int n);
void 	Print_Board(int* board, int size);
// Prototype Tree functions
struct	TREE* Create_Tree(int n);		// Create Tree with n roots
int Add_Node(struct q_node* node, int row, int column, int n);
void Print_Queens(struct q_node* node, int row, int n);
void Delete_Tree(struct TREE *tree, int n);
void Delete_Nodes(struct q_node* node, int n);

// MAIN
int main(void) {

	// Local Variables
	int n;						// number of queens wanting to place
	int num_queens = 0;			// number of queens placed
	int *board;
	int data[2];
	struct TREE * tree;			//
	struct q_node * current;
	int error = 0;				// error checker
	int row = 0;				// row and column values for board marking
	int column = 0;				//
	double timer;
	clock_t start, end;			// timer variables
	int counter = 0;			// check how many time progam goes through loop

	// START
	// Query for the number of queens and intro
	printf("Welcome to nqueens \n");
	while (!error)
	{
		printf("Enter a number of queens greater than 0: ");
		fflush(stdout);
		scanf("%d", &n);
		fflush(stdin);
		// board values greater than 0
		if (n > 0)
		{
			break;
		}
	}

	// Create board of size nxn
	board = Create_Board(n);

	// Start Timer
	start = clock();

	//
	tree = Create_Tree(n);
	current = tree->root;

	// Start queens and keep going until rows are over amount
	while (row < n)
	{
		//Print_Board(board,n);
		//fflush(stdout);
		// check row and column
		if (Check_Queen(row, column, board, n) && column < n)
		{
			// assign queen in that position
			board[row * n + column] = 1;

			// Add branch/leaf to tree
			Add_Node(current, row, column, n);
			current = current->branch[column];
			fflush(stdout);
			// Increment row and prepare column for increment to origin
			row++;
			column = -1;
			num_queens++;
			Print_Queens(current, num_queens, n);

		}// end if
		while (column >= n)
		{
			// data output is row and column
			data[0] = current->data[0];
			data[1] = current->data[1];
			// tranverse back up tree since no solution
			current = current->parent;

			if (row == 0 && column >= n)
			{

				printf("No moves remaining \n");
				return 0;
				Print_Queens(current, num_queens, n);
			}
			else
			{
				// Set the row and column
				row = data[0];
				column = data[1];
				board[data[0] * n + data[1]] = 0;
				num_queens--;
			}
		} // while column
		counter++;
		column++;
		current->solution = 1;
		//printf("column: %d \n", column);
	} // while row

	end = clock();
	timer = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time: %f Count: %d\n", timer, counter);

	Delete_Tree(tree, n);
	Print_Board(board, n);
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
	for (i = row; i >= 0; i--)
	{
		// Check Rows
		if (board[i * size + column] != 0)
		{
			//printf("Row Hit %d: %d: %d \n", i, column, row);
			return 0;
		}
	}

	// Check to see if the left diagonal is guarded
	j = column;
	for (i = row; i >= 0; i--)
	{
		// check if the left diagonal checker is out of bounds
		if (j < 0)
			break;

		if ((board[i * size + j] != 0))
		{	//printf("Left diagonal \n");
			return 0;
		}
		j--;
	}

	// Check to see if the right diagonal is guarded
	j = column;
	for (i = row; i >= 0; i--)
	{
		// Check if the right diagonal checker is out of bounds
		if (j >= size)
			break;

		if ((board[i * size + j] != 0))
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
	int *board = (int*)malloc(n*n * sizeof(int));

	// Check if board allocation fails
	if (board == NULL)
	{
		printf("Board allocation failed \n");
		fflush(stdout);
		return board;
	}

	// assign the value of zero for each element
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			board[i * n + j] = 0;
	}

	// return board of size nxn;
	return board;
}

/* Create Tree
 *	Input: N/A
 *	Output: A Tree
 */
struct TREE* Create_Tree(int n)
{
	// Local Variables
	struct TREE * tree;
	int i;

	// allocate memory
	tree = (struct TREE *)malloc(sizeof(struct TREE));
	if (tree)
	{
		//tree->num = 0;			// Number of nodes
		tree->root = (struct q_node *)malloc(n * sizeof(struct q_node));
		tree->root->solution = -1;	// Dummy value to indicate beginning of tree
		tree->root->branch = (struct q_node **)malloc(n * sizeof(struct q_node *)); // Assign memory to the branches of the branch
		for (i = 0; i < n; i++)
			tree->root->branch[i] = NULL;
	}
	else
	{
		printf("Not enough memory to allocate tree \n");
		fflush(stdout);
		//return tree;
	}
	return tree;
}

/* insert_Tree
 * 	Input: pointer to stack, row value and column value for new queen
 *	Output:Queen data inserted into stack
 */
int Add_Node(struct q_node* node, int row, int column, int n)
{
	// Local Variables
	struct q_node *new_node;
	int i;

	// Allocate memory for new node
	new_node = (struct q_node*)malloc(sizeof(struct q_node));
	// if true then allocation failure
	if (new_node == NULL)
		return 0;

	// Assign queen properties
	new_node->data[0] = row;
	new_node->data[1] = column;
	new_node->parent = node;			// assign parent branch
	new_node->branch = (struct q_node **)malloc(n * sizeof(struct q_node *)); // Assign memory to the branches of the branch
	for (i = 0; i < n; i++)
		new_node->branch[i] = NULL;
	new_node->solution = 0; // solution at 1 leaf so assume it isn't
	//
	node->branch[column] = new_node;
	fflush(stdout);
	return 1;
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
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("%d ", board[i * size + j]);
		}
		printf("\n");
	}

}

/* Print Queens
 * Output: Queens and their positions
 */
void Print_Queens(struct q_node* node, int queens, int n)
{
	// Local Variables
	int i;

	//start from top (last to first)
	for (i = queens; i > 0; i--)
	{
		printf("Queen: %d, row: %d, column: %d \n", i, node->data[0] + 1, node->data[1] + 1);
		fflush(stdout);
		node = node->parent;	// last link of the chain is NULL
	}

	// Print unplaced queens //
	for (i = n; i > queens; i--)
	{
		printf("Queen: %d, row: -1, column: -1 \n", i);
		fflush(stdout);
	}
	printf("End Move \n");

}

/* Delete Tree & Delete Nodes
 *
 * Output: Delete all leaves and branches
 */
void Delete_Tree(struct TREE *tree, int n)
{
	// Local Variables
	int i;

	// See if the Tree is initialized
	if (tree)
	{
		// Delete the root's branches and then recursively delete branches
		for (i = 0; i < n; i++)
			Delete_Nodes(tree->root->branch[i], n);
		free(tree->root);
	}
	// if tree is empty
	else
	{
		return;
	}

}
void Delete_Nodes(struct q_node* node, int n)
{
	// Local Variables
	int i;
	if (node)
	{
		// Delete each branch
		for (i = 0; i < n; i++)
		{
			// if the branch is allocated in memory space
			if (node->branch[i])
			{
				Delete_Nodes(node->branch[i], n);
			}
		}
	}
	// if the space is NULL
	else
	{
		return;
	}
	// Free the branch pointers
	free(node->branch);
	// free tree node
	free(node);
	return;
}
