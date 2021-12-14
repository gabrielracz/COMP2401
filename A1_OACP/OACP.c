/*
===============================================================================
Author:			Gabriel Racz
Version:		1.1
Features:		System administrator inputs 3 character control sequence 
				representing the execute, write, and read permissions
				of an object.

===============================================================================
*/
#include<stdio.h>

//Each bit in the ACD is used as a boolean flag, relating to y/n in the input character sequence
//Bits are written in the same order as the input sequence, ynn -> 100
int setBits(char* ACD, const char* usrInput, int len){
	for(int i = 0; i < len; i++){
		int n = len-1-i;
		if (usrInput[i] == 'y')
			*ACD = *ACD | (1 << n);
		else if(usrInput[i] == 'n')
			*ACD = *ACD & (~(1 << n));
		else
			return 1;
	}
	return 0;
}

//Reads each bit in the ACD char and prints to console
int printBits(char ACD){
	int size = sizeof(ACD)*8;
	int bit;
	printf("b:  ");
	for(int i = 0; i < size; i++){
		bit = (ACD & (1 << size - 1 - i)) >> size - 1 - i;
		printf("%d", bit);
	}
	printf("\n");
	return 0;
}

int main(void){
	//Collect character sequence from user
	printf("\nObject Access Control Program (OACP)\nWelcome System_Administrator.\nEnter 3 digit character sequence (y/n):\n");
	char usrInput[3];
	scanf("%c%c%c", &usrInput[0],  &usrInput[1],  &usrInput[2]);
	printf("\n");
	
	//Save character sequence y/n data to char
	char ACD = 0;
	
	if(!setBits(&ACD, usrInput, 3)){
		//Display ACD in various formats
		printf("Permissions saved.\nDisplaying saved data:\n");
		printf("c:  %c\n", ACD);
		printf("d:  %d\n", ACD);
		printBits(ACD);	
	}else{
		printf("Error: Invalid input.\nPermissions not saved.\n");
	}
	return 0;
}
