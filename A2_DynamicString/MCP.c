/*
	Author:			Gabriel Racz 101181470
	Date:			06/13/2021
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct TransactionType{
	char	TID[11];
	char	CID[8];
	float 	amount;
}Transaction;

float convertAmountToFloat(char* amt){
	float rslt = 0;
	float flt = atof(amt);		//atof turns string to float
	return flt;
}

int processInputString(Transaction*** tList, int* count){
	char ch;
	*count = 0;
	Transaction** new_tList;
	while((ch = getchar()) == '|'){
		*count += 1;
		//Allocate another space for a transaction
		*tList = (Transaction**) realloc(*tList, sizeof(void*) * (*count));		//Change reallocate tList to have one extra space
		

		Transaction* curTran = (Transaction*) malloc(sizeof(Transaction));		//New transaction on the heap

		char TID[11];
		fgets(TID, 11, stdin);
		strcpy(curTran->TID, TID);
		if(getchar() != ','){
			return 1;
		}

		char CID[9];
		fgets(CID, 9, stdin);
		strcpy(curTran->CID, CID);
		if(getchar() != ','){
			return 2;
		}

		char amtString[11];
		fgets(amtString, 11, stdin);
		if(strlen(amtString) != 10)
			return 3;
		float amtFloat = convertAmountToFloat(amtString);
		curTran->amount = amtFloat;

		*(*tList + (*count) - 1) = curTran;		//*tList is the pointer to the first element of the list
	}

	if(ch != EOF && ch != '\n'){
		printf("\nOffending character: '%c'", ch);
		return 3;
	}
	return 0;
}


int main(void){	
	Transaction** tList = (Transaction**) malloc(sizeof(Transaction*));
	unsigned int count;
	printf("Enter message payload:\n");
	int errorCode;
	errorCode = processInputString(&tList, &count);
	
	switch(errorCode){			//Print return code
		case 0:
			printf("\nMessage payload returned with error code 0\n");
			break;
		case 1:
			printf("\nINPUT ERROR: Transaction ID in transaction #%d\n", count);
			count--;
			break;
		case 2:
			printf("\nINPUT ERROR: Customer ID in transaction #%d\n", count);
			count--;
			break;
		case 3:
			printf("\nINPUT ERROR: Amount in transaction #%d\n", count);
			count--;
			break;
		default:
			break;
	}
	
	printf("Displaying processed results:\n\n");
	float sum = 0;
	for(int i = 0; i < count; i++){
		printf("Transaction ID: %10s\nCustomer ID:    %10s\nAmount:        $%10.2f\n\n", tList[i]->TID, tList[i]->CID, tList[i]->amount);
		sum += tList[i]->amount;
		free(tList[i]);		//Free each transaction
	}
	float average = sum / count;
	printf("\nNumber of Transactions: %d\nAverage Transaction Amount: $ %.2f\n\n", count, average);
	free(tList);	//Free the entire array
	return 0;
}

