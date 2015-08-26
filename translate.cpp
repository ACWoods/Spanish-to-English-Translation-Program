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
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <sys\types.h>
#include <winsock2.h>


#define NUM_CL_ARGS 2
#define MAXCHARSIZE	255
#define MAXSIZE 255
#define PORTNUMBER 5254		/* Arbitrary port assignment*/

using namespace std;


void main(int argc, char *argv[])
{
	if((argc < NUM_CL_ARGS) || !strcmp(argv[1], "/?") || !strcmp(argv[1], "/0"))	/*Error checks command line arguments and prints error message if there are insufficient arguments or if user inputs "/?".*/
	{		
		cout<<"English to Spanish Translation Program"<<endl;
		cout<<"Usage: translate [English_word]"<<endl;
		exit(0);
	}	/*end if*/

	char test[] = "cat", user_word_input[MAXCHARSIZE], translated_word[MAXCHARSIZE], outfilename[] = "translation.txt";
	int i = 0, n = 0;
	struct sockaddr_in server_address;		/*sockaddr_in (defined in winsock2.h) is a structure that contains Internet address information*/
	struct hostent *server;		/*'server' is a pointer to the hostent structure, which defines a host computer on the Internet (address information in struct members)*/
	
	/***Windows Socket declarations***/
	SOCKET socket_1 = INVALID_SOCKET;
	WSADATA wsaData;


	fstream outfile;	/*fstream creates a buffer for input or output. ofstream could be used instead to specify use as an output.*/
	
	outfile.open(outfilename, fstream::out);
	
	if(outfile.is_open() == false)		/*Error checking opening of output file*/
	{	
		cout<<"ERROR: file "<<outfilename<<" can not be opened!"<<endl;
		exit(0);
	}	/*end if*/



	/***Initializing Windows Sockets Version 2.2***/
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)		
		printf("ERROR: Program could not initialize Windows Sockets.\n");

	socket_1 = socket(AF_INET, SOCK_STREAM, 0);		/*Creating the socket: AF_INET for Internet address domain, SOCK_STREAM for continuous stream socket type, and 0 to choose TCP as protocol for stream*/
	if(socket_1 < 0)
		printf("ERROR: Socket could not be opened.\n");
	
	server = gethostbyname("127.0.0.1");		/*Takes name of server ('dictionary') and returns a pointer to a hostent structure (defined 'server') containing information about the host*/
	if(server == NULL)
	{
		printf("ERROR: Host not found.\n");
		exit(0);
	}	/*end if*/
	
	memset(&server_address, 0, sizeof(server_address));	/*Initializing the server address buffer to 0*/

	/***Setting the fields in server_address***/
	server_address.sin_family = AF_INET;
	memmove(&server_address.sin_addr.s_addr,
		server->h_addr,		/*h_addr is defined as the first address in the array h_addr_list*/
		server->h_length);

	server_address.sin_port = htons(PORTNUMBER);		/*htons() converts a port number in host byte order to a port number in network byte order*/

	cout<<"\n\n\n***English to Spanish Translation Program***\n\n\n";
	cout<<"Input word is: "<<argv[1]<<endl;

//	strcpy(user_word_input, test);		/*Debugging purposes-passing a word guaranteed to be in dictionary*/
	strcpy(user_word_input, argv[1]);
	

	if(connect(socket_1, (sockaddr *) &server_address, sizeof(server_address)) < 0)		/*Establishing connection to server with error checking*/
		printf("ERROR: Socket could not be connected to server.\n");

	
	n = send(socket_1, user_word_input, (int) sizeof(user_word_input), 0);	/*Sending information to the server on the socket*/
	if (n == SOCKET_ERROR)
	{
		printf("ERROR: No information sent on socket.\n");
		closesocket(socket_1);
		return;
	}	/* end if*/

	n = recv(socket_1, translated_word, (int) sizeof(translated_word), 0);	/*Receiving information from the server on the socket*/
	if (n == SOCKET_ERROR)
	{
		printf("ERROR: No information received on socket\n");
		closesocket(socket_1);
		return;
	}	/* end if*/

	cout<<"Translated word is: "<<translated_word<<endl;
	outfile<<user_word_input<<"\t\t"<<translated_word<<endl;

	closesocket(socket_1);

	cout<<"\n\nTranslation output to "<<outfilename<<endl;

}	/*end main*/