#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_STR 50
#define SERVER_PORT 1337
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

int pokemonSearch(char* type, Pokemon*** matches);
void sig_handler(int signo);
void clearKeyboardBuffer(void);
Pokemon* parsePokemon();

FILE* file_in = NULL;
int serverSocket;


int main(int argc, char* argv[]){
  	int                 clientSocket;
  	struct sockaddr_in  serverAddress, clientAddr;
  	int                 status, addrSize, bytesRcv;
  	char                buffer[30];
  	char*               response = "OK";
 
	char filename[MAX_STR];
	int rc;
	
	//STARTUP
	printf("____________POKESERVER____________ \n");
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
	
	//========================================SERVER SETUP=====================================//
  	//Create the server socket
  	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  	if (serverSocket < 0) {
    	printf("*** SERVER ERROR: Could not open socket.\n");
    	exit(-1);
  	}

  	//Setup the server address
  	memset(&serverAddress, 0, sizeof(serverAddress)); 						//zeros the struct
  	serverAddress.sin_family = AF_INET;
  	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  	serverAddress.sin_port = htons((unsigned short) SERVER_PORT);

  	//Bind the server socket
  	status = bind(serverSocket,  (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  	if (status < 0) {
    	printf("*** SERVER ERROR: Could not bind socket.\n");
    	exit(-1);
  	}

  	//Set up the line-up to handle up to 5 clients in line 
  	status = listen(serverSocket, 5);
  	if (status < 0) {
    	printf("*** SERVER ERROR: Could not listen on socket.\n");
    	exit(-1);
  	}
	//========================================================================================//

  	// Wait for clients now
  	while(1){
		signal(SIGINT, sig_handler); 		//catch sigint
    	addrSize = sizeof(clientAddr);
    	clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrSize);
    	if (clientSocket < 0) {
      		printf("*** SERVER ERROR: Could not accept incoming client connection.\n");
      		exit(-1);
    	}
    	printf("SERVER: Received client connection.\n");

    	bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);			//Get the type request
    	buffer[bytesRcv] = 0; 												//Null terminator
    	printf("SERVER: Received client request: %s\n", buffer);

		/*POKEMON SEARCH*/
		int index;
		Pokemon** matches = malloc(sizeof(Pokemon*) * 30); 
		index = pokemonSearch(buffer, &matches);
	
		// Respond with Pokemon records
    	printf("SERVER: Sending search results to client\n");
     
	 	int converted_index = htonl(index);
		send(clientSocket, &converted_index, sizeof(converted_index), 0);	//Send how many Pokemon records will be sent

	  	for(int i = 0; i < index; i++){
	  		Pokemon* p = matches[i];
			int converted_int;
			char converted_char;
			char output[150];
			char response[3];
			
			
			sprintf(output, "#%-3d  %-30s  %9s %-9s  |%d|%-3hhu|%-3hhu|%-3hhu|%-3hhu|%-3hhu|%-3hhu|  |%hhu| |%hhu|", p->number, p->name, p->type1, p->type2, p->total, p->hp, p->attack, p->defense, p->spatk, p->spdef, p->speed, p->generation, p->legendary);
			send(clientSocket, output, strlen(output), 0);					//Send the record string
			recv(clientSocket, &response, sizeof(response), 0);				//Recieve the 'OK' to send more
			
			free(p);
	  	}
	  	free(matches);
		printf("SERVER: Closing client connection.\n\n");
    	close(clientSocket); 												//Close this client's socket

	}
	return 0;
}


int pokemonSearch(char* type, Pokemon*** matches){
	int length = 30;	
	int index = 0;
	

	//SEARCH POKEMON FILE
	fseek(file_in, 0, SEEK_END);
	long end = ftell(file_in);
	fseek(file_in, sizeof(char) * 87, SEEK_SET); 							//Seek after header
	

	Pokemon* p;
	while(ftell(file_in) < end - 1){
		p = parsePokemon();

		if(!strcmp(p->type1, type)){
			(*matches)[index] = p;
			index++;
			if(index == length){
				length = length*2;
				*matches = realloc(*matches, sizeof(Pokemon*) * length);	//Dynamically size the array	
			}
		}else
			free(p);
	}

	return(index);
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

void sig_handler(int signo){
	if(signo == SIGINT){
		close(serverSocket);
		if(file_in != NULL)
			fclose(file_in);
	}
  	printf("\nSERVER: Shutting down.\n");
	exit(0);
}

void clearKeyboardBuffer(void){
	char ch;
	while(ch = getchar() != '\n'&& ch != EOF);			//Keep reading chars until stdin gives EOF
}

