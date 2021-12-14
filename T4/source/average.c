/* File is average.c
 *
 * Purpose: 
 * test the function average()
 *
 */

#include "stdio.h"

// This is a forward reference
int average(int x, int y, int *average);


int main(int argc, char **argv)

{
	int x;
	int y;
	int result;

	printf("Testing the average function \n");

	// test 1 
	x = 5;
	y = 7;
	printf("test 1: testing average(%d, %d) answer should be 6\n",x,y);
	average(x,y, &result);
 	printf("average(%d, %d)=%d\n \n",x, y, result);

	// test 2 
	x = 5;
	y = 4;
	printf("test 2: testing average(%d, %d) answer should be 4.5\n",x,y);
	average(x,y, &result);
 	printf("average(%d, %d)=%d\n \n",x, y, result);

	return(0);
}


/* computes the average of two numbers
 * input:
 * x - the first number 
 * y - the last number  
 *
 * output:
 * average - the average of the two numbers
 *
 * return:
 * none 
 *
 *  
 */

int average(int x, int y, int *average) {

	int sum;
	int i;

	sum = 0; 
  
	// compute the sum
	sum = x;
	for (i = 0; i < y; i++) {
		sum += 1;
	}

	// compute the average
	*average = sum /2;
    
	return (0);
}
