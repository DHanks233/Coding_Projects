//--------------------------------------------------------------------------
// File:        driver.c
//
// Functions:   int main()
//              int userMenu()
//              void clearBuffer()
//              void openFile()
//              void countArray()
//              void readArray()
//              void swapPointers()
//              void printOutNextGen ()
//              void getNextGen()
//              void clearOutput()
//				void clearArrays
//				void bearWave()
//				void autoRun()
//--------------------------------------------------------------------------

#include "project_4.h"

//--------------------------------------------------------------------------
//Function:     main()
//
// Description: Allows the user to play the Game of Life by prompting them
//              for input, providing error feedback regarding incorrect input
//              or files too large, calculates the next generation according
//              to Game of Life rules and prints that generation
//
// Environment:
// Hardware:    Intel i7-3770K
//              Intel i7-3632QM
//              MacBook Pro
//              AMD Phenom II X6 1045T
//
// Software:    Windows 7 Ultimate 64-bit
//              Windows 7 Home
//              VM Fusion
//              Windows 8 Pro
//
// Input:       A char value used to determine if the loop iterating the
//              program should continue or quit
//
// Output:      Various error prompts and a prompt to continue the program
//              or quit
//
// Parameters:  int argc: The number of files read in from the command line
//              char *argv[]: An array of pointers to strings that contains
//              filenames entered via the command line
//
// Returns:     An int value, either EXIT_SUCCESS or EXIT_FAILURE
//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int i = 0;
	int userChoice = 0;
    int rows = 0;
    int columns = 0;
    char userQuit = 'y';
    FILE * inputFilename = NULL;
    char filenameString[MAX_STRING] = "";
   	char currentGen[LARGE_BOUNDS][LARGE_BOUNDS] = {{" "} , {" "}};
   	char nextGen[LARGE_BOUNDS][LARGE_BOUNDS] = {{" "} , {" "}};
	char (*pointer1)[LARGE_BOUNDS] = currentGen;
    char (*pointer2)[LARGE_BOUNDS] = nextGen;
   	char (*tempPointer)[LARGE_BOUNDS] = NULL;
	char tempArray[LARGE_BOUNDS][LARGE_BOUNDS] = {{" "} , {" "}};  

    while(userQuit == tolower('y'))
    {
        userChoice = userMenu();
        openFile(argc, argv, userChoice, filenameString);
        inputFilename = fopen(filenameString, "r");
        if(inputFilename == NULL)
        {
            printf("\nCould not find an input file with the name %s.", 
				filenameString);
            printf("\n\nPlease press Enter to exit.");
            getchar();
            return EXIT_FAILURE;
        }
        
		clearArrays(currentGen, nextGen);
        countArray(&rows, &columns, inputFilename);
        if(rows > LARGE_BOUNDS || columns > LARGE_BOUNDS)
        {
            printf("\nYour array is larger than allowed by this program.");
            fclose(inputFilename);
            userQuit = playAgain();
            continue;
        }
        
        readArray(rows, columns, inputFilename, currentGen, nextGen);
		printf("\nWould you like the program to run automatically?"
			"\nEnter 'y' or 'n': ");
		if(tolower(getchar()==  'y'))
        {
            autoRun(pointer1, pointer2, rows, columns, currentGen);
        }
		
		clearBuffer();
		do
        {
		    printOutNextGen(pointer1, rows, columns);
            getNextGen(pointer1, pointer2, columns,  rows);
            swapPointers(&pointer1 , &pointer2 , currentGen);
			printf("\n\nPress Enter to see next Generation or another key "
				"then Enter to quit. ");
            
        }while(getchar() == '\n');
        
        clearOutput();
        clearBuffer();
        userQuit = playAgain();
    }
    
    printf("Thank you for playing The Game of Life!");
    printf("\n\nBrought to you by the loving char bears...");
	getchar();
    fclose(inputFilename);
    bearWave( inputFilename, currentGen, nextGen, tempArray);
	printf("\nGoodbye! Press Enter to exit program.");
    getchar(); 

    return EXIT_SUCCESS;
}