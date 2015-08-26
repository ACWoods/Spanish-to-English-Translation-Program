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


using namespace std;

void main(int argc, char *argv[])
{
	if((argc < NUM_CL_ARGS) || !strcmp(argv[1], "/0"))	/*Exit if no value was passed*/
	{		
		cout<<"ERROR: No argument passed to dictionary program. Program will now exit."<<endl;
		exit(-1);
	}	/*end if*/

	int i = 0, n = 0, j = 0, m = 0, word_not_found = 0, length_of_user_word = 0;
	char c, translated_word[MAXCHARSIZE], user_word[MAXCHARSIZE], line[MAXCHARSIZE], compare_buffer_input[MAXCHARSIZE], infilename[] = "dictionary.txt";


	FILE *in_file = NULL;	/*Opens input file and output file buffers*/
	in_file = fopen(infilename,"r");

	if (in_file == NULL)		/*Error checking opening of input file*/
	{
		cout<<"ERROR: file "<<infilename<<" can not be opened!"<<endl;
		exit(0);
	}	/*end if*/

	strcpy(user_word, argv[1]);		/*Copying passed word to the string 'user_word'*/
	length_of_user_word = strlen(user_word);


	/***WORD LOOKUP***/
	while(fgets(line, MAXCHAR, in_file) != NULL)		/* Read each line of the dictionary until the end of file has been reached*/
	{

		for(i = 0; !isspace(compare_buffer_input[i]); i++)		/*Places English words in the dictionary in a comparison buffer for comparison with the user input word*/
		{
			compare_buffer_input[i] = line[i];
			if(isspace(compare_buffer_input[i]))
				break;
		}	/*end for*/
		compare_buffer_input[i] = '\0';
		


		if(!strcmp(compare_buffer_input, user_word))		/*If user word matches dictionary word, then output the second string of the line, aka the translated word*/
		{
			for(j = i; line[j] != '\0'; j++)
			{
				c = line[j];
				if(isspace(c))		/*Don't start copying until the end of whitespace is reached, i.e. the translated word on the lineis found*/
					continue;
				else
					break;
			}	/*end for*/

			for(m = 0; line[j] != '\0'; m++)		/*Begin copying the translated word*/
			{
				if(line[j] == '\n')
				{
					translated_word[m] = '\0';		/*Getting rid of the auto carriage return inserted by fgets and null terminating the string*/
					break;
				}	/*end if*/
				else
				{
					translated_word[m] = line[j];
					j++;
				}	/*end else*/
			}	/*end while*/

			word_not_found = 0;
			break;
		}	/*end if*/
		else		/*If word pulled from dictionary does not match user word, go to the next line*/
		{
			word_not_found = 1;
			continue;
		}	/*end else*/
	}	/*end while*/

	if(word_not_found == 1)		/*If user word is not found in the dictionary, return "no match*/
	{
		sprintf(translated_word, "<NO MATCH FOUND>");
	}	/*end if*/


	/*send word back to usm through pipe*/
	fputs(translated_word, stdout);		/*Takes the translated word and puts it on the standard output. Serves two purposes: prints word on screen and allows it to be captured by parent.*/
	fflush(stdout);		/*Drives 'translated_word' to file buffer stdout immediately*/


} /*end main*/



