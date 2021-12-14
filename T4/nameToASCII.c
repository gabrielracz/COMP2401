#include<stdio.h>
#include<string.h>
#define MAX_STR 100


int main(void){
	char name[MAX_STR];
	printf("Enter your name: ");
	scanf("%100s", &name);
	int len = strlen(name);
	for(int i = 0; i < len; i++){
		printf("%c -> %d\n", name[i], name[i]);
	}	

	return 0;
}
