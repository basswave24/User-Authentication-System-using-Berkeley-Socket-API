//Name: Dan Andrei Pirlea / Student Number: C18444682

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#include <unistd.h>

static const int MAXPENDING = 5; // This stores the maximum outstanding connection requests

#define PROCEED "PROCEED" //this #define stores the string "proceed" which we will send if the user successfuly authenticates 
#define DENIED "DENIED" // this #define stores the "denied" string which we will send if the user went over 3 attempts

int main(int argc, char *argv[]) {

	char sendbuffer[BUFSIZE]; // Buffer for sending data to the client 
	char recvbuffer[BUFSIZE]; // Buffer for receiving data from the client
	
	int numBytes = 0; //This is used as the return value for recv() 
	
	//These 2 characters arrays will store the users details
	char username[20];
	char password[20];
	
	if (argc != 2) // This ensures the user doesn't add too many parameters, it should only be the server port (eg: ./authServer 8080)
		DieWithUserMessage("Parameter(s)", "<Server Port>");
	
	in_port_t servPort = atoi(argv[1]); // This stores the port of the server , a parameter given by the user 
	
	// Create socket for incoming connections
	int servSock; // This is the server socket 
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) //AF_INET refers to IPV4 addresses, sock_stream is used by TCP and IPPROTO_TCP corresponds to TCP
		DieWithSystemMessage("socket() failed"); //error checking
	
	// Construct local address structure
	struct sockaddr_in servAddr;                  // Server address structure
	memset(&servAddr, 0, sizeof(servAddr));       // We initialise everything with zero using memset function
	servAddr.sin_family = AF_INET;                // IPv4 address family
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Accepts any incoming interface AND converts from host byte order to network byte order LONG using htonl
	servAddr.sin_port = htons(servPort);          // Stores local port and converts from host byte order to network byte order SHORT
	
	// Bind to the local address we just created above 
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) //Everything must be passed by reference
		DieWithSystemMessage("bind() failed"); //error checking
		
	// Marking the socket so it will listen for incoming connections
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed"); //error checking
	
	for (;;) { // Infinite for loop; this will run forever, accepting any incoming connections
	
	int attempts = 3; //this variable stores the amount of attempts the user can get to log in.
	
	//CONNECTION HAPPENING *
	int clntSock = accept(servSock, (struct sockaddr *) NULL, NULL); //Accepts the connection and sends a message back to the client 
    if (clntSock < 0)                                                //(Cont.)It creates a new socket descriptor with the same properties as servSocket
      DieWithSystemMessage("accept() failed");                       //(Cont.)And returns it back to the client 
	
		
	

	while(attempts != 0) //receive data and send data as long as attempts is not zero
	{	
		attempts--; //we decrement it by 1 since each iteration counts as one attempt
		while ((numBytes = recv(clntSock, recvbuffer, BUFSIZE - 1, 0)) > 0) //this will execute while data is being received from the clntSock onto recvBuffer
		{
			recvbuffer[numBytes] = '\0'; //indicates termination of a character string 
			fputs(recvbuffer, stdout); //this writes the string to the terminal BUT NOT including the null character 
			
			if(strstr(recvbuffer, "\r\n\r\n") > 0) //when it finds this (which basically means 2 new lines), it will break out of the while loop
				break;                             //since it reached the end of what it's currently transmitting
		}
		
		sscanf(recvbuffer, "%s %s ", username, password); //reads formatted input from a string ( ie: looks for username[] and password[] passed by authClient 
														  //onto the arrays on our server
		
		
		if(!strcmp(username,"admin") && !strcmp(password,"pass")) //return value from strcmp is ZERO, thats why were using ! , so the if can execute 
		{                                                     //compares the string username with "admin" and string password with "pass" and proceeds if they're equal
			snprintf(sendbuffer, sizeof(sendbuffer), "%s \r\n", PROCEED); //Placing the PROCEED string we defined at the top of the program onto sendbuffer AS WELL as \r\n because the client expects that so he can stop receiving data from server side
			ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0); //we send over the message to clntSock
			
			if (numBytesSent < 0) //can't be less than zero, if it is , theres an issue with send() such as failing to be sent 
				DieWithSystemMessage("send() failed");  //we will be doing this for every single send as part of error checking
			
			break; //since the user is authenticated, we just wanna break from the while loop
		}
			
		if(attempts == 0) //if the user has no more attempts
			{
				snprintf(sendbuffer, sizeof(sendbuffer), "%s \r\n", DENIED);//Placing the DENIED string we defined as well as \r\n to indicate the end of the send
				ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0);//sending ofer the message
				
				if (numBytesSent < 0)
					DieWithSystemMessage("send() failed");
				
				break;// we can just break out of the while loop since the user got denied permission anyway
			}
		
		//else...(if there are attempts left)
		snprintf(sendbuffer, sizeof(sendbuffer), "You have %d attempt(s) left \r\n", attempts);//Placing how many attempts left as well as \r\n(to indicate end) onto the send buffer
		ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0); //sending over the buffer
		
		if (numBytesSent < 0)
			DieWithSystemMessage("send() failed");
		
		
	}//end while
			close(clntSock); //we close the connection with the client sock when we are done

	}//end for loop

	
}
	
	
	

