#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include<semaphore.h>
#include<pthread.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
 
#define MAX_STR 50
#define  SERVER_IP   "127.0.0.1" 
#define  SERVER_PORT 1337 
 
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


void* pokemonServerQuery(void* type);
void clearKeyboardBuffer(void);
void capitalizeString(char* str);
void printPokemon(Pokemon* p, FILE* out);
Pokemon* parsePokemon(char* file_in);
void* saveMaster(void* file_out);

sem_t mutex;
char** masterList;
volatile int masterCount;
volatile int masterSize;

struct sockaddr_in  serverAddress; 

int main() { 
	/*========================================================SERVER CONNECTION=============================================*/	
 	int                 status, bytesRcv; 
 	char                inStr[80];  // stores user input from keyboard 
 	char                buffer[80]; // stores user input from keyboard 
 	
 
  	// Setup address 
  	memset(&serverAddress, 0, sizeof(serverAddress)); 
  	serverAddress.sin_family = AF_INET; 
  	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP); 
  	serverAddress.sin_port = htons((unsigned short) SERVER_PORT); 
 
 	/*=======================================================================================================================*/
	
	int rc;

	//INIT GLOBALS
	masterSize = 75;
	masterCount = 0;
	masterList = malloc(sizeof(char*) * masterSize);
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


  	// Go into loop to commuincate with server now 
		printf(" PokeDexplorer V2 \n");
  	while (1) {  
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
				pthread_create(&searchThreads[searches - 1], NULL, pokemonServerQuery, type);
			}else{
				pthread_join(searchThreads[searches % 5], NULL);								//Wait for oldest thread to return
				searches++;
				pthread_create(&searchThreads[(searches - 1) % 5], NULL, pokemonServerQuery, type);
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

			//SAVE FILE HISTORY	
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
  			printf("CLIENT: Shutting down.\n"); 
			return 0;
		} 
	}
	return 0;
}

void* saveMaster(void* file_out){
	fprintf(file_out, " Num  Name                                   Typing        Tot  Hp Atk Def Satk Sdef Spd  Gen Leg\n");
	
	if(sem_wait(&mutex) < 0){
		printf("Error: on semaphore wait - save\n");
		exit(1);
	}
	for(int i = 0; i < masterCount; i++){
		fprintf(file_out, "%s\n", masterList[i]);			//Write pokemon record string to save file
	}
	if(sem_post(&mutex) < 0){
		printf("Error: on semaphore post - save\n");
		exit(1);
	}
	
	fclose(file_out);
	return 0;
}

void* pokemonServerQuery(void* type){
 	int status;
	// Create the client socket 
  	int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
 	if (clientSocket < 0) { 
		printf("*** CLIENT ERROR: Could not open socket.\n"); 
    	exit(-1); 
  	}

  	// Connect to server 
  	status = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)); 
  	if (status < 0) { 
  	 	 printf("*** CLIENT ERROR: Could not connect.\n"); 
   	 	exit(-1); 
  	} 
    
	send(clientSocket, type, strlen(type), 0); //Send the type
	int bytesRcv;
	
	int num_matches;
	recv(clientSocket, &num_matches, sizeof(num_matches), 0);			//Recieve amount of pokemon records to be transfered
	num_matches = ntohl(num_matches);

	if(sem_wait(&mutex) < 0){											//Global var thread protection
		printf("Error: on semaphore wait - search\n");
		exit(1);
	}
	for(int i = 0; i < num_matches; i++){
		int rcvd_int;
		char rcvd_char;
		char buffer[150] = {0};
		int bytesRcvd;
		char response[] = "OK";


		bytesRcvd = recv(clientSocket, buffer, sizeof(buffer), 0);		//Recieve pokemon record
		send(clientSocket, &response, sizeof(response), 0);				//Respond OK
		buffer[bytesRcvd] = '\0';

		char* pokemonRecord = malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy(pokemonRecord, buffer);

		//printf("%s\n", buffer);	
		//usleep(100000);

		masterList[masterCount] = pokemonRecord;						//Save to masterList of all records
		masterCount++;
		if(masterCount == masterSize){									//Dynamically size the array
			masterSize *= 2;
			masterList = realloc(masterList, sizeof(char*) * masterSize);
			if(masterList == NULL){
				printf("Error: on masterList realloc - search\n");
			}
		}
	}
	if(sem_post(&mutex) < 0){
		printf("Error: on semaphore post - search\n");
		exit(1);
	}
	
	close(clientSocket);
	return 0;
	
}

Pokemon* parsePokemon(char* file_in){
	Pokemon* p = (Pokemon*) malloc(sizeof(Pokemon));	
	char ch = '\0';
	char c;
	char index = 0;
	int rc;
	
	//Number
	sscanf(file_in, "%d", &p->number);
	sscanf(file_in, "%c", &c); //comma

	//Name
	sscanf(file_in, "%c", &ch);
	while(ch != ','){						
		p->name[index] = ch;
		index++;
		sscanf(file_in, "%c", &ch);
	}
	p->name[index] = '\0';
	index = 0;
	
	//Type1
	sscanf(file_in, "%c", &ch);
	while(ch != ','){
		p->type1[index] = ch;
		index++;
		sscanf(file_in, "%c", &ch);
	}
	p->type1[index] = '\0';
	index = 0;

	//Type2
	sscanf(file_in, "%c", &ch);
	while(ch != ','){
		p->type2[index] = ch;
		index ++;
		sscanf(file_in, "%c", &ch);
	}
	p->type2[index] = '\0';
	index = 0;
	
	//Stats
	sscanf(file_in, "%d %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c %hhu %c", &p->total, &c, &p->hp, &c, &p->attack, &c, &p->defense, &c, &p->spatk, &c, &p->spdef, &c, &p->speed, &c, &p->generation, &c); 		//data

	//Translate the True/False into 1/0
	char lgndString[6];
	sscanf(file_in, "%s", lgndString);
	if(lgndString[0] == 'T')
		p->legendary = 1;
	else
		p->legendary = 0;
	
	return p;
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
