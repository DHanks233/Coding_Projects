//--------------------------------------------------------------------------
// File:		Project4.h
//
// Functions:	int userMenu()
//				void clearBuffer()
//				void clearFileBuffer()
//				int safeReadInt()
//				void openFile()
//				void getInputFile()
//				void countArray()
//				void readArray()
//				void swapPointers()
//				void printOutNextGen ()
//				void getNextGen()
//				void clearOutput()
//				char playAgain()
//				void clearArrays()
//				void swapPointers()
//				void autoRun()
//				void bearWave()
//--------------------------------------------------------------------------

#ifndef PROJECT_4_H
#define PROJECT_4_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>
#pragma warning(disable: 4996)

#define MAX_STRING 100
#define ERROR_PROMPT "\nPlease enter a valid selection: "
#define CHOICE_ONE 1
#define CHOICE_TWO 2
#define CHOICE_THREE 3
#define CHOICE_FOUR 4
#define CHOICE_FIVE 5
#define CHOICE_INPUT 6
#define LARGE_BOUNDS 40
#define ALIVE '*'
#define DEAD ' '


//--------------------------------------------------------------------------
// Outputs a menu to user in stdout and takes in the user's choice
//--------------------------------------------------------------------------
int userMenu(void);

//--------------------------------------------------------------------------
// Clears the input buffer
//--------------------------------------------------------------------------
void clearBuffer(void);

//--------------------------------------------------------------------------
// Uses fgetc() to take characters from the input file until it reaches \n
//--------------------------------------------------------------------------
void clearFileBuffer(FILE * inputFilename);

//--------------------------------------------------------------------------
// Validates user input in the menu to allow only an integer of 1 to 5
//--------------------------------------------------------------------------
int safeReadInt(int inputNumber, char *errorPrompt);

//--------------------------------------------------------------------------
// Takes the user's menu choice and opens the appropriate file, or allows
// user to input their own test file
//--------------------------------------------------------------------------
void openFile(int numberFiles, char *userFilename[], int userChoice, 
	char filenameString[]);

//--------------------------------------------------------------------------
// Opens an input file from argv, or by getting input from user in stdin
//--------------------------------------------------------------------------
void getInputFile(int numberFiles, char *filenames[], char inputFilename[]);

//--------------------------------------------------------------------------
// Counts the number of rows and maximum number of columns in the input file
//--------------------------------------------------------------------------
void countArray(int *rows, int *columns, FILE * inputFilename);

//--------------------------------------------------------------------------
// Reads the text file into a 40 x 40 array, and fills in any rows that have
// less than the maximum number of columns
//--------------------------------------------------------------------------
void readArray(int rows, int columns, FILE * inputFilename, 
	char (*currentGen)[LARGE_BOUNDS], char (*nextGen)[LARGE_BOUNDS]);

//--------------------------------------------------------------------------
// Outputs the current contents of a 2-D array to stdout
//--------------------------------------------------------------------------
void printOutNextGen (const char (*nextGen)[LARGE_BOUNDS],  int rows,  
	int columns);

//--------------------------------------------------------------------------
// Reads each cell of the currentGen 2d array.  Based on the rules of the 
// Game of Life, each corresponding cell of the nextGen 2d array is written
// with either ALIVE '*' or DEAD ' '.
//--------------------------------------------------------------------------
void getNextGen(const char (*currentGen)[LARGE_BOUNDS], 
	char (*nextGen)[LARGE_BOUNDS], int colMax, int rowMax);

//--------------------------------------------------------------------------
// A windows system command that clears the output from stdout
//--------------------------------------------------------------------------
void clearOutput(void);

//--------------------------------------------------------------------------
// Prompts user to quit or play again, takes choice and clears buffer
//--------------------------------------------------------------------------
char playAgain(void);

//--------------------------------------------------------------------------
// Sets all cells in each array to DEAD (a space character)
//--------------------------------------------------------------------------
void clearArrays(char (*pointer1)[LARGE_BOUNDS], 
	char (*pointer2)[LARGE_BOUNDS]);

//--------------------------------------------------------------------------
// Swaps addresses of the two pointers in main that point to currentGen and 
// nextGen 2-D arrays
//--------------------------------------------------------------------------
void swapPointers(char (**pointer1)[LARGE_BOUNDS], 
	char (**pointer2)[LARGE_BOUNDS], char (*currentGen)[LARGE_BOUNDS]);

//----------------------------------------------------------------------------
// Runs the program until keyboard input is detected
//----------------------------------------------------------------------------
void autoRun(char (*pointer1)[LARGE_BOUNDS], char (*pointer2)[LARGE_BOUNDS], 
	int rows, int columns, char (*currentGen)[LARGE_BOUNDS]);

//-----------------------------------------------------------------------------
// Displays waving char bear before exiting program
//-----------------------------------------------------------------------------
void bearWave( FILE * inputfile, char inputArray1[][LARGE_BOUNDS],
	char inputArray2[][LARGE_BOUNDS], 
	char tempArray[LARGE_BOUNDS][LARGE_BOUNDS]);

#endif
