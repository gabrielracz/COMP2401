#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main() {
  int                 clientSocket, addrSize, bytesReceived;
  struct sockaddr_in  clientAddr;

  char                inStr[80];    // stores user input from keyboard
  char                buffer[80];   // stores sent and received data

  // Create socket
  clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (clientSocket < 0) {
    printf("*** CLIENT ERROR: Could open socket.\n");
    exit(-1);
  }

  // Setup address 
  memset(&clientAddr, 0, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  clientAddr.sin_port = htons((unsigned short) SERVER_PORT);

  // Go into loop to commuincate with server now
  while (1) {
    // Get a command from the user
    printf("CLIENT: Enter a positive integer or \"done\" or \"stop\" commands to send to primeServer ... ");
    scanf("%s", inStr);
	printf("\n");
    //fgets(inStr, sizeof(inStr), stdin);
    //inStr[strlen(inStr)-1] = 0;

    // Send command string to server
    strcpy(buffer, inStr);
    printf("CLIENT: Sending \"%s\" to server.\n", buffer);
    sendto(clientSocket, buffer, strlen(buffer), 0,
           (struct sockaddr *) &clientAddr, sizeof(clientAddr));

    // Get response from server, should be "OK"
    addrSize = sizeof(clientAddr);
    bytesReceived = recvfrom(clientSocket, buffer, 80, 0,
		        (struct sockaddr *) &clientAddr, &addrSize);
    buffer[bytesReceived] = 0; // put a 0 at the end so we can display the string
    printf("CLIENT: Got back response \"%s\" from server.\n", buffer);
    
    if ((strcmp(inStr,"done") == 0) || (strcmp(inStr,"stop") == 0))
      break;
  } 

  close(clientSocket);  // Don't forget to close the socket !
  printf("CLIENT: Shutting down.\n");
}
