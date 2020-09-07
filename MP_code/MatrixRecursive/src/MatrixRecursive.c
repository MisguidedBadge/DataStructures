/*
 ============================================================================
 Name        : MatrixRecursive.c
 Author      : Trey
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

//#define RANDOM_MAX 10000

// Function Prototypes
int enter_size();
int* create_matrix(int m);
struct maxval matrix_max(int m, int *matrix, int r_max, int c_max, int r_min, int c_min, int rowbyte);			// Recursive funtion
struct maxval maxtest(struct maxval a, struct maxval b);
// GLOBAL VAR
long int counter;	// iteration counter

// Structures
// matrix value structure
struct maxval{
	int val;
	int row;
	int column;
};

int main(void) {
// LOCAL VAR INST and DEC
	int *matrix;
	int m;
	struct maxval max;
	clock_t start, end;
	double timer;
// START
	srand(time(0));
	printf("Matrix Max Program :D \n");

	// enter matrix size
	m = enter_size();
	if (m == -1)
		return 0;

	// create and populate matrix
	matrix = create_matrix(m);
	//printf(" %d %d \n", matrix[1], matrix[2]);

	start = clock();
	// determine maximum of the matrix
	max = matrix_max(m, matrix, m, m, 0, 0, m);

	end = clock();
	timer = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("row: %d column: %d, value: %d \n", max.row, max.column, max.val);
	printf("unrolling counter: %ld, Time: %f \n", counter, timer);
	free(matrix);
	// EXIT
	return 0;
}

/*	Enter Size
 *
 *	Input:
 *	Output: Size M of power of 2
 *
 */
int enter_size()
{
// local variables
	int m, i;
	i = 0;

// Query the user for a power of 2 number
	while(i < 100)
	{
		printf("Enter a power of 2 for the matrix size \n");
		fflush(stdout);		// flush stream due to eclipse IDE
		scanf("%d", &m);

		// Check if m is a power of 2 using bitwise operations
		// for example subtract by 1 to go from 1000 to 0111 then and it to get 0
		//printf("%x \n", (m & (m - 1)));	// print
		if((!(m & (m - 1))) && m != 0)
		{	printf("Done \n");
			break;	}
		else
		{
			printf("Matrix size invalid \n");
		}
		i++;
	}
	// give up after 100 tries
	if(i == 100)
		return -1;
	else
		return m;
}

/*	Create Matrix
 *
 * 	Input: size m that of which is a power of 2
 *	Output: randomized matrix of mxm
 *
 *	allocate a continous block of memory to avoid fragmentated allocation
 *
 */
int* create_matrix(int m)
{
	// local Variables
	int i, j;

	// allocate a block of memory for a 2D matrix
	int *matrix = (int*)malloc(m*m*sizeof(int));

	/*
	    TEST
		matrix[1] = 2;
		matrix[2] = 3;
		printf("%d %d \n", matrix[1], matrix[2]);
	*/

	// assign random numbers to each element
	for( i = 0; i < m; i ++)
	{	for ( j = 0; j < m; j ++)
		{
			matrix[i * m + j] = rand();
		}
	}
	/* Display values for testing purposes
	for( i = 0; i < m; i ++)
	{	for ( j = 0; j < m; j ++)
		{
			printf("%d ", matrix[i * m + j]);
		}
		printf("\n");
	}
	*/

	return matrix;
}


/*	Matrix Max
 *
 *	input:  Matrix of size mxm and value m
 *	output: Row and Column of maximum value in a structure
 *
 *	Baseline check if 1x1 and then output row column
 *
 *	Slicing the matrix requires boundaries (min to max of each column and row), the m value, and the matrix passed
 */
struct maxval matrix_max(int m, int *matrix, int r_max, int c_max, int r_min, int c_min, int rowbyte)
{
	// local variables
	struct maxval max;
	struct maxval a, b, c, d;	// four different options
	struct maxval temp1, temp2;
	counter++;
	// basline
	if(m == 1)
	{
		//printf("%d \n", matrix[(rowbyte * r_min) + c_min]);
		//fflush(stdin);
		max.val = matrix[(rowbyte * r_min) + c_min];
		max.row = r_min + 1;
		max.column = c_min + 1;
		return max;
	}
	// Partition into 4 sub matrices and determine the maximum value within those sub matrices
	else
	{
		a =	 matrix_max(m/2, matrix, m/2 - 1 + r_min, m/2 - 1	+ c_min, 0		+ r_min, 0		+ c_min,rowbyte);	// top left
		b =	 matrix_max(m/2, matrix, m/2 - 1 + r_min, m - 1		+ c_min, 0		+ r_min, m/2	+ c_min,rowbyte);	// top right
		c =	 matrix_max(m/2, matrix, m - 1	 + r_min, m/2 - 1	+ c_min, m/2	+ r_min, 0		+ c_min,rowbyte);	// bottom left
		d =	 matrix_max(m/2, matrix, m - 1	 + r_min, m - 1		+ c_min, m/2	+ r_min, m/2	+ c_min,rowbyte);	// bottom right
	}
	// compares the 4 matrix values and then returns the largest one
	temp1 = maxtest(a, b);
	temp2 = maxtest(c, d);
	max = maxtest(temp1, temp2);
	return max;
}

/* Max Value Test
 *
 *	Input: two structures
 *	Output: Structure with the bigger value
 */
struct maxval maxtest(struct maxval a, struct maxval b)
{
	// compare value of each structure and return the bigger one
	if( a.val >= b.val)
		return a;
	else
		return b;
}
