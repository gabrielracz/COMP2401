#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define MAX_STR 50

typedef struct PokemonType{
	int number;
	char name[MAX_STR];
	char type1[10];
	char type2[10];
	int total;
	char hp;
	char attack;
	char defense;
	char spatk;
	char spdef;
	char speed;
	char generation;
	char legendary;
}Pokemon;

void* pokemonSearch(void* type);
void printPokemon(Pokemon* p, FILE* out);
Pokemon* parsePokemon();
void* saveMaster(void* file_out);
void clearKeyboardBuffer(void);
void capitalizeString(char* str);


sem_t mutex;
Pokemon** masterList;
volatile int masterCount;
volatile int masterSize;
FILE* file_in;


int main(int argc, char* argv[]){
	char filename[MAX_STR];
	int rc;
	
	printf("___________POKEDEXPLORER___________ \n");
	//INPUT FILE
	printf("Enter the name of the Pokemon file: \n");
	scanf("%49s", filename);
	clearKeyboardBuffer();
	file_in = fopen(filename, "r");
	
	while(!file_in){
		printf("Pokemon file is not found. Please enter the name of the file again:\n");
		scanf("%s", filename);
		file_in = fopen(filename, "r");
	}
	printf("Pokemon file found.\n\n");

		
	//INIT GLOBALS
	masterSize = 75;
	masterCount = 0;
	masterList = malloc(sizeof(Pokemon*) * masterSize);
	if(masterList == NULL){
		printf("Error: on masterList malloc\n");
		exit(1);
	}

	rc = sem_init(&mutex, 0, 1);
	if(rc < 0){
		printf("Error: on semaphore init\n");
	}

	//THREAD CONTAINERS
	pthread_t searchThreads[5];
	int searches = 0;

	pthread_t saveThreads[3];
	int saves = 0;

	//MENU VARIABLES
	int saveSize = 5;
	char** saveHistory = malloc(sizeof(char*) * saveSize);	

	int choice;


	while(1){
		printf("      MENU      \n");
		printf("[1] Type search\n[2] Save results\n[3] Exit\n");
		scanf("%d", &choice);
		printf("\n");
		
	//SEARCH
		if(choice == 1){
			char type[10];
			printf("Enter type:\n");
			scanf("%9s", type);
			clearKeyboardBuffer();
			capitalizeString(type);
			printf("\n");
		
			//MANAGE SEARCH THREAD POOL
			if(searches < 5){
				searches++;
				pthread_create(&searchThreads[searches - 1], NULL, pokemonSearch, type);
			}else{
				pthread_join(searchThreads[searches % 5], NULL);								//Wait for oldest thread to return
				searches++;
				pthread_create(&searchThreads[(searches - 1) % 5], NULL, pokemonSearch, type);
			}
	//SAVE
		}else if(choice == 2){
			char filename_out[MAX_STR];
			printf("Enter save file name:\n");
			scanf("%49s", filename_out);
			printf("\n");
			clearKeyboardBuffer();
			FILE* file_out = fopen(filename_out, "w");
			while(file_out == NULL){
				printf("Error: Unable to create the file %s. Enter the name of the file again:\n", filename_out);
				scanf("%49s", filename_out);
				clearKeyboardBuffer();
				file_out = fopen(filename_out, "w");
			}
			if(saves == saveSize - 1){
				saveSize *= 2;
				saveHistory = realloc(saveHistory, sizeof(char*) * saveSize);
				if(saveHistory == NULL){
					printf("Error: on saveHistory realloc\n");
					exit(1);
				}
			}
			
			char* save_record = malloc(sizeof(char) * 50);
			strcpy(save_record, filename_out);
			saveHistory[saves] = save_record;

			//MANAGE SAVE THREAD POOL
			if(saves < 3){
				pthread_create(&saveThreads[saves], NULL, saveMaster, file_out);
				saves++;
			}else{
				pthread_join(saveThreads[saves % 3], NULL);										//Wait for oldest thread to return
				pthread_create(&saveThreads[saves % 3], NULL, saveMaster, file_out);
				saves++;
			}
	//EXIT
		}else if(choice == 3){
			//CLEANUP THREADS
			if(searches != 0){
				int openThreads;
				if(searches > 5)
					openThreads = 5;
				else
					openThreads = searches;
				
				for(int i = 0; i < openThreads; i++){
					pthread_join(searchThreads[i], NULL);
				}
			}
			
			if(saves != 0){
				int openSaveThreads;
				if(saves > 3)
					openSaveThreads = 3;
				else
					openSaveThreads = saves;
				
				for(int i = 0; i < openSaveThreads; i++){
					pthread_join(saveThreads[i], NULL);
				}
			}
				
			printf("Queries completed: %d\n", saves);
			printf("Save file(s) created:\n");
			for(int i = 0; i < saves; i++){
			printf(" -%s\n", saveHistory[i]);
				free(saveHistory[i]);			//Free the save file records
			}
			printf("\n");
			free(saveHistory);

			//CLEANUP MEMORY
			for(int i = 0; i < masterCount; i++){
				free(masterList[i]);
			}
			free(masterList);
			fclose(file_in);
			
			return 0;
		}
	}
	return 0;
}

void* pokemonSearch(void* type){
	int length = 30;	
	Pokemon** matches = malloc(sizeof(Pokemon*) * length); 
	int index = 0;
	

	//SEARCH POKEMON FILE
	fseek(file_in, 0, SEEK_END);
	long end = ftell(file_in);
	fseek(file_in, sizeof(char) * 87, SEEK_SET); 		//Seek after header
	

	Pokemon* p;
	while(ftell(file_in) < end - 1){
		p = parsePokemon(file_in);

		if(!strcmp(p->type1, type)){
			matches[index] = p;
			index++;
			if(index == length){
				length = length*2;
				matches = realloc(matches, sizeof(Pokemon*) * length);
			}
		}else
			free(p);
	}
	
	//SAVE MATCHES TO MASTERLIST	
	if(sem_wait(&mutex) < 0){
		printf("Error: on semaphore wait - search\n");
		exit(1);
	}
	for(int i = 0; i < index; i++){
		masterList[masterCount] = matches[i];
		masterCount++;
		if(masterCount == masterSize){					//Dynamically size the array
			masterSize *= 2;
			masterList = realloc(masterList, sizeof(Pokemon*) * masterSize);
			if(masterList == NULL){
				printf("Error: on masterList realloc - search\n");
			}
		}
	}
	if(sem_post(&mutex) < 0){
		printf("Error: on semaphore post - search\n");
		exit(1);
	}
	
	free(matches);
	return(0);
}

Pokemon* parsePokemon(){
	Pokemon* p = (Pokemon*) malloc(sizeof(Pokemon));	
	char ch = '\0';
	char c;
	unsigned short field = 1;
	char index = 0;
	int rc;
	
	//Number
	fscanf(file_in, "%d", &p->number);
	fscanf(file_in, "%c", &c); //comma

	//Name
	fscanf(file_in, "%c", &ch);
	while(ch != ','){						
		p->name[index] = ch;
		index++;
		fscanf(file_in, "%c", &ch);
	}
	p->name[index] = '\0';
	index = 0;
	
	//Type1
	fscanf(file_in, "%c", &ch);
	while(ch != ','){
		p->type1[index] = ch;
		index++;
		fscanf(file_in, "%c", &ch);
	}
	p->type1[index] = '\0';
	index = 0;

	//Type2
	fscanf(file_in, "%c", &ch);
	while(ch != ','){
		p->type2[index] = ch;
		index ++;
		fscanf(file_in, "%c", &ch);
	}
	p->type2[index] = '\0';
	index = 0;
	
	//Stats
	fscanf(file_in, "%d %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c", &p->total, &c, &p->hp, &c, &p->attack, &c, &p->defense, &c, &p->spatk, &c, &p->spdef, &c, &p->speed, &c, &p->generation, &c); 		//data

	//Translate the True/False into 1/0
	char lgndString[6];
	fscanf(file_in, "%s", lgndString);
	if(lgndString[0] == 'T')
		p->legendary = 1;
	else
		p->legendary = 0;
	
	return p;
}

void* saveMaster(void* file_out){
	fprintf(file_out, " Num  Name                                   Typing        Tot  Hp Atk Def Satk Sdef Spd  Gen Leg\n");
	
	if(sem_wait(&mutex) < 0){
		printf("Error: on semaphore wait - save\n");
		exit(1);
	}
	for(int i = 0; i < masterCount; i++){
		printPokemon(masterList[i], file_out);
	}
	if(sem_post(&mutex) < 0){
		printf("Error: on semaphore post - save\n");
		exit(1);
	}
	
	fclose(file_out);
	return 0;
}

void printPokemon(Pokemon* p, FILE* out){
	fprintf(out, "#%-3d  %-30s  %9s %-9s  |%d|%-3hhu|%-3hhu|%-3hhu|%-3hhu|%-3hhu|%-3hhu|  |%hhu| |%hhu|\n", p->number, p->name, p->type1, p->type2, p->total, p->hp, p->attack, p->defense, p->spatk, p->spdef, p->speed, p->generation, p->legendary);
}

void clearKeyboardBuffer(void){
	char ch;
	while(ch = getchar() != '\n'&& ch != EOF);
}

void capitalizeString(char* str){
	
	if(str[0] >= 'a' && str[0] <= 'z')
		str[0] -= ('a' - 'A');
		
	for(int i = 1; i < strlen(str); i++){
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += ('a' - 'A');
	}
	return;
}

 
