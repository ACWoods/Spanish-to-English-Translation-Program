/******************************************************************
 * Program #: Assignment 3: User Session Manager
 *
 * Programmer: Allen Woods
 *
 * Due Date: November 4, 2013
 *
 * CMSC 355, Fall 2013       Instructor: Dr. Robert Dahlberg
 *
 * Pledge: I have neither given nor received unauthorized aid on this program.
 *
 * Description: Translate.exe takes an English word at the command line from the user, passes it to the user service manager which passes it to the dictionary program, and returns the Spanish equivalent in an output file using socket communications.; C++ implementation.
 *
 * Inputs: user-defined word
 *
 * Outputs: Translated word in Spanish in output file
 *
 ******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctype.h>
#include <fstream>
#include <sys\types.h>
#include <winsock2.h>
#include <windows.h>

#define NUM_CL_ARGS 2
#define MAXCHARSIZE	255
#define MAXBACKLOG 5
#define PORTNUMBER 5254		/* Arbitrary port assignment*/


using namespace std;

void main(void)
{
	int n = 0, client_addr_length = 0, length_of_user_word = 0;
	char child_args[MAXCHARSIZE], user_word[MAXCHARSIZE], translated_word[MAXCHARSIZE];

	struct sockaddr_in server_address, client_address;		/*sockaddr_in (defined in winsock2.h) is a structure that contains Internet address information*/

	/*Windows Socket declarations*/
	SOCKET socket_1 = INVALID_SOCKET, acceptsocket_1 = INVALID_SOCKET;		/*socket_1 will be a listening socket; acceptsocket_1 will be a new connected socket for the first pending connection on the listening socket*/
	WSADATA wsaData;


	/***Initializing Windows Sockets Version 2.2***/
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)		
		printf("ERROR: Program could not initialize Windows Sockets.\n");

	socket_1 = socket(AF_INET, SOCK_STREAM, 0);		/*Creating the socket: AF_INET for Internet address domain, SOCK_STREAM for continuous stream socket type, and 0 to choose TCP as protocol for stream*/
	if (socket_1 < 0)
		printf("ERROR: Socket could not be opened.\n");

	memset(&server_address, 0, sizeof(server_address));	/*Initializing the server address buffer to 0*/
	
	/***Setting the fields in server_address***/
	/*The sockaddr_in structure specifies the address family, IP address, and port for the socket that is being bound.*/
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORTNUMBER);		/*htons() converts a port number in host byte order to a port number in network byte order*/


	/***Setting up connection to client***/
	client_addr_length = sizeof(client_address);
	n = bind(socket_1, (struct sockaddr *) &server_address, sizeof(server_address));		/*Binds socket to server IP address and port*/
	if(n == SOCKET_ERROR)		
		printf("ERROR: Failure on binding: Code %d", WSAGetLastError());

    listen(socket_1, MAXBACKLOG);		/*Server listens to the socket for connections*/


	while(1)		/*Server runs continuously*/
	{
		acceptsocket_1 = accept(socket_1, (struct sockaddr *) &client_address, &client_addr_length);		/*accept() pauses the server process until a client connection is made*/
		if (acceptsocket_1 == SOCKET_ERROR)
			printf("ERROR: Socket was not accepted: Code %d", WSAGetLastError());

		n = recv(acceptsocket_1, user_word, (int) sizeof(user_word), 0);	/*Receiving information from the client on the socket*/
		if (n == SOCKET_ERROR)
		{
			printf("Error receiving on socket.");
			closesocket(acceptsocket_1);
			return;
		}	/* end if*/
		
		_snprintf(child_args, sizeof(child_args), "dictionary.exe %s", user_word);		/*Putting all arguments for dictionary's main function into an array*/


        FILE *child_service = _popen(child_args, "r");		/*Opening the child process and setting its standard output to 'child_service'*/

		fgets(translated_word, MAXCHAR, child_service);		/*Reads the string on the file buffer child_service and places it in 'translated_word'. Previously used fscanf but could not read whitespace.*/
		n = send(acceptsocket_1, translated_word, (int) sizeof(translated_word), 0);	/*Sending information to the client on the socket*/
		if (n == SOCKET_ERROR)
		{
			printf("Error sending on socket.");
			closesocket(acceptsocket_1);
			return;
		}	/* end if*/	

		_pclose(child_service);

	}	/* end while*/

}	/*end main*/