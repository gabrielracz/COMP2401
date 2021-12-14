#include <stdio.h>
#include "bit_functions.h"

void printBitsIterative(unsigned char num){
	int bits = 8;
	int bit;
	int n;
	for(int i = 0; i < bits; i++){
		n = bits - i - 1;
		bit = (num & (1 << bits - i - 1)) >> bits - i - 1;
		printf("%d", bit);
	}
	printf("\n");
}

void printBitsRecursive(unsigned char c){
	if(c == 1 || c == 0){
		printf("%d", c);
		return;
	}

	printBitsRecursive(c >> 1);
	
	printf("%d", isBitSet(c, 1));
}


int isBitSet(unsigned char num, int bitNum){
	return((num &  (1 << bitNum-1)) >> bitNum-1);
}

unsigned char setBit(unsigned char num, int bitNum){
	num = num | (1 << bitNum - 1);
	return num;
}

unsigned char clearBit(unsigned char num, int bitNum){
	return(num & (~(1 << bitNum - 1)));
}



int main() 
{
    unsigned char a = 'A';

    printBitsIterative(a);
    printf("setting bits 2 and 3 \n");
    a = setBit(a, 2);
    a = setBit(a, 3);
    printBitsIterative(a);

	//printf("\n%d\n", isBitSet(0b00000100, 2));

    printf("clearing bit 2 \n");
    a = clearBit(a, 2);
   //	printBitsIterative(a);
	printBitsRecursive(a);
	printf("\n");
    return 0;
}
 

