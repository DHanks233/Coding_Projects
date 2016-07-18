//-------------------------------------------------------------------------- 
// File:        Project_4.c 
// 
// Functions:   int userMenu() 
//              void clearBuffer() 
//              void clearFileBuffer() 
//              int safeReadInt() 
//              void openFile() 
//              void getInputFile() 
//              void countArray() 
//              void readArray() 
//              void swapPointers() 
//              void printOutNextGen () 
//              void getNextGen() 
//              void clearOutput() 
//              char playAgain() 
//              void clearArrays()
//				void swapPointers()
//				void autoRun()
//				void bearWave()
//-------------------------------------------------------------------------- 
  
#include "project_4.h" 
  
//-------------------------------------------------------------------------- 
//Function:     userMenu() 
// 
// Description: Outputs a menu to stdout for user, and takes returns user's 
//              choice 
// 
// Input:       User's choice from the output menu 
// 
// Output:      Displays a menu for user 
// 
// Returns:     int: The number user chose from the output menu 
//-------------------------------------------------------------------------- 
int userMenu(void) 
{ 
    int userChoice = 0; 
  
    printf("Welcome to the Game of Life!\n\n"
		"Select a number from the menu to open the corresponding file.\n\n"
		"(1) Tumbler\n"
		"(2) Quad Tumbler\n"
		"(3) Air Traffic Control\n"
		"(4) Glider Gun\n"
		"(5) Char Bear\n"
		"(6) Open another input file\n\n"
        "Your choice: "); 
    userChoice = safeReadInt(userChoice, ERROR_PROMPT); 
  
    return userChoice; 
} 
  
//-------------------------------------------------------------------------- 
//Function:     clearBuffer() 
// 
// Description: Eliminates any extra characters in the buffer until reaching 
//              the end of the line 
//-------------------------------------------------------------------------- 
void clearBuffer(void) 
{ 
    if(getchar() != '\n') 
    { 
        while(getchar() != '\n') 
        { 
            continue; 
        } 
    } 
} 
  
//-------------------------------------------------------------------------- 
//Function:     clearFileBuffer() 
// 
// Description: Eliminates any extra characters in the input file until  
//              reaching the end of the line 
// 
// Parameters:  FILE * inputFilename: The pointer to the text file being used 
//              for input 
//-------------------------------------------------------------------------- 
void clearFileBuffer(FILE * inputFilename) 
{ 
    char valueRead = '\0'; 
    while((valueRead = fgetc(inputFilename) == DEAD) || (valueRead == ALIVE)) 
    { 
        continue; 
    } 
} 
  
//-------------------------------------------------------------------------- 
//Function:     safeReadInt() 
// 
// Description: Takes in user's choice, prompts user to re-enter if the input 
//              is not an integer value of 1 through 6. 
// 
// Input:       An integer value representing user's choice from the 
//              displayed menu 
// 
// Output:      An error prompt if the function encounters incorrect input 
// 
// Parameters:  int inputNumber:    An int designed to take in user's choice 
//              char *errorPrompt:  A string that outputs to user they have 
//                                  entered incorrect input 
//-------------------------------------------------------------------------- 
int safeReadInt(int inputNumber, char *errorPrompt) 
{ 
    scanf("%d", &inputNumber); 
    while(inputNumber <= 0 || inputNumber > 6) 
    { 
        fputs(ERROR_PROMPT, stdout); 
        clearBuffer(); 
        scanf("%d", &inputNumber); 
    } 
    clearBuffer(); 
  
    return inputNumber; 
} 
  
//-------------------------------------------------------------------------- 
//Function:     openFile() 
// 
// Description: The function opens the appropriate file based on user's 
//              choice, or opens getInputFile to allow user to enter 
//              the name of their own input file 
// 
// Parameters:  int numberFiles:      The argc value read from command line
//              char *userFilename[]: The array of pointers from argv[] 
//              int userChoice:       The user's choice from the menu 
//              char inputFilename[]: The string used to store the input 
//                                    filename entered by user 
//-------------------------------------------------------------------------- 
 void openFile(int numberFiles, char *userFilename[], int userChoice,  
     char inputFilename[]) 
{ 
    strcpy(inputFilename, ""); 
    switch(userChoice) 
    { 
        case CHOICE_ONE: strcpy(inputFilename, "Tumbler20x40.txt"); 
                            break; 
        case CHOICE_TWO: strcpy(inputFilename, "quadTumbler30x40.txt");
                            break; 
        case CHOICE_THREE: strcpy(inputFilename, 
							   "airTrafficControl40x40.txt");
                            break; 
        case CHOICE_FOUR: strcpy(inputFilename, "gliderGun20x40.txt"); 
                            break; 
        case CHOICE_FIVE: strcpy(inputFilename, "charBears40x40.txt"); 
                            break; 
        case CHOICE_INPUT: getInputFile(numberFiles, userFilename, 
							   inputFilename); 
                            break; 
    } 
} 
  
//-------------------------------------------------------------------------- 
//Function:     getInputFile()
// 
// Description: Takes in the name of user's own input file, either by reading 
//              the name from argv[] in the command line, or by allowing user 
//              to enter their own filename at the prompt. 
// 
// Input:       Name of the input file read as a string 
// 
// Output:      Prompts user for input filename if not already entered via 
//              the command line 
// 
// Parameters:  int numberFiles:      The argc value representing number of 
//                                    files read from the command line 
//              char *userFilename[]: The argv[] values 
//              char inputFilename[]: The string to hold the filename input 
//                                    by user from stdin 
//-------------------------------------------------------------------------- 
void getInputFile(int numberFiles, char *userFilename[],  
    char inputFilename[]) 
{ 
    printf("\nYour text file cannot be larger than 40 rows x 40 columns."); 
    if(numberFiles > 1) 
    { 
        strncpy(inputFilename, userFilename[1], MAX_STRING - 1); 
    } 
    else
    { 
        printf("\nEnter the input filename: "); 
        fgets(inputFilename, MAX_STRING - 1, stdin); 
        if(inputFilename[strlen(inputFilename) - 1] == '\n') 
        { 
            inputFilename[strlen(inputFilename) - 1] = '\0'; 
        } 
        else
        { 
            clearBuffer(); 
        } 
    } 
} 
  
//-------------------------------------------------------------------------- 
//Function:     countArray() 
// 
// Description: Counts the number of rows from the input file, and finds the 
//              maximum number of columns used in a row for the whole file 
// 
// Compiles under Microsoft Visual Studio 2010 
// 
// Input:       Reads from the text input file opened 
// 
// Parameters:  int * rows:    Number of rows contained in the input file 
//              int * columns: Number of columns contained in the input file 
//              FILE * inputFilename: A pointer to the opened input file 
//-------------------------------------------------------------------------- 
void countArray(int *rows, int *columns, FILE * inputFilename) 
{ 
    int maxColumns = 0; 
    char valueRead = '\0'; 
    *rows = 0; 
    *columns = 0; 
  
    while(fgetc(inputFilename) != EOF) 
    { 
        (*rows)++; 
        (*columns) = 1; /*Since we've read in one value, set the number of 
						columns to 1 */
        while((valueRead = fgetc(inputFilename)) == DEAD 
			|| valueRead == ALIVE)
        { 
                (*columns)++; 
        } 
        if(*columns > maxColumns) 
        { 
            maxColumns = *columns; 
        } 
    } 
    *columns = maxColumns; 
    rewind(inputFilename); 
} 
  
//-------------------------------------------------------------------------- 
//Function:     readArray() 
// 
// Description: Reads the file into two 40 x 40 2-dimensional arrays based on
//              rows and columns parameters. If one row has less columns then
//              the parameter columns, the remainder of that row is filled
//              with spaces to represent dead cells. 
// 
// Input:       The opened text file is read for input 
// 
// Parameters:  int rows:    The number of rows contained in the file 
//              int columns: The number of columns contained in the file 
//              inputFilename: The pointer to the text file being used for 
//              input 
//              char (*currentGen)[LARGE_BOUNDS): Pointer to currentGen[][]
//              char (*nextGen)[LARGE_BOUNDS): pointer to nextGen[][] 
//-------------------------------------------------------------------------- 
void readArray(int rows, int columns, FILE * inputFilename,  
    char (*currentGen)[LARGE_BOUNDS], 
    char (*nextGen)[LARGE_BOUNDS]) 
{ 
    int i = 0; 
    int j = 0; 
    char valueRead = '\0'; 
  
    for(i = 0; i < rows; i++) 
    { 
        for(j = 0; j < columns; j++) 
        { 
            valueRead = fgetc(inputFilename); 
            if(valueRead == '\n') //If row terminates early 
            { 
                while(j < columns) 
                {   //Fill remainder of row with spaces 
                    currentGen[i][j] = DEAD; 
                    nextGen[i][j] = DEAD; 
                    j++; 
                } 
			}
            if(valueRead != ALIVE)
            {  /*If a character not supported by this program is read, change
				it to DEAD*/
                    currentGen[i][j] = DEAD; 
                    nextGen[i][j] = DEAD;
			}
			else
			{
            currentGen[i][j] = valueRead; 
            nextGen[i][j] = valueRead; 
			}
        } 
		if(valueRead != '\n') 
		{ 
           clearFileBuffer(inputFilename); 
        }
     }

} 
 
  
//-------------------------------------------------------------------------- 
//Function:     printOutNextGen() 
// 
// Description: Prints out the 2-D array being used to store the next 
//              generation in the Game of Life. The function also prints a
//				border to show the bounds of the array during execution,
//				by using several putchar() calls
// 
// Output:      Displays the 2-D array containing the characters for the Game 
//              of Life 
// 
// Parameters:  const char (*nextGen)[LARGE_BOUNDS]: A pointer to the 2-D 
//                                      array containing the next generation 
//              int rows:       The number of rows used in the 2-D array 
//              int columns:    Number of columns used in the 2-D array 
//-------------------------------------------------------------------------- 
void printOutNextGen (const char (*nextGen)[LARGE_BOUNDS], int rows, 
	int columns) 
{ 
    int i = 0; 
    int j = 0;
	int k = 0;
  
    clearOutput();
	putchar(201);
	for( k = 1; k < columns+1; k++)
	{
		putchar(205);
	}

	putchar(187);
	putchar('\n');
    for(i = 0; i < rows; i++) 
    { 	
		putchar(186);
        for(j = 0; j < columns; j++) 
        { 
            putchar(nextGen[i][j]); 
        } 
		putchar(186);
        putchar('\n'); 
    } 
	putchar(200);
	for( k = 1; k < columns+1; k++)
	{
		putchar(205);
	}
	putchar(188);
} 
  
//-------------------------------------------------------------------------- 
//Function:     getNextGen() 
// 
// Description: Reads each cell of the currentGen 2d array.  Based on the
//              rules of the Game of Life, each corresponding cell of the 
//              nextGen 2d array is written with either ALIVE '*' or DEAD ' '
//              Rules of The Game of Life: 
//                  If a cell of currentGen has exactly 2 ALIVE neighbors, 
//                  it's status is unchanged in nextGen. 
//                  If a cell of currentGen has exactly 3 ALIVE neighbors, 
//                  it will be ALIVE in nextGen. 
//                  Otherwise, the cell in nextGen will be DEAD. 
//              Cell diagram: 
//              1  2  3 
//              4  C  5 
//              6  7  8 
//       
// Parameters:  const char (*currentGen)[LARGE_BOUNDS]: 2d array to be read 
//              char (*nextGen)[LARGE_BOUNDS]: 2d array to be written 
//              int colMax:  number of columns being used 
//              int rowMax:  number of rows being used 		
//-------------------------------------------------------------------------- 
 
 void getNextGen(const char (*currentGen)[LARGE_BOUNDS],
	 char (*nextGen)[LARGE_BOUNDS],  int colMax, int rowMax)
{
	int row = 0;  //row counter
    int col = 0;  //column counter
    int neighbors = 0;  //neighbor counter
	int i = 0; // row neighbor search counter
	int j = 0; // column neighbor search counter

	for (row = 0; row < rowMax; row++)
    {
	for (col = 0; col < colMax; col++)
	{
		for(i = 1; i >= -1 ; i--)
		{
		for(j = -1; j <= 1; j++)
		{	/*If the cell being evaluated falls within the bounds of the 
			array*/
			if((row+i)>=0 && (row+i)<rowMax && (col+j)>=0 && (col+j)<colMax)
			{//Do not count the cell itself as a neighbor
			if(!(i==0 && j==0))
			{
			if(currentGen[row+i][col+j]==ALIVE)
			{
				neighbors++;				
			}}}
		}}
		switch(neighbors)
		{
			case 2: //Cell survives
				nextGen[row][col] = currentGen[row][col];
				neighbors = 0;
				break;
			case 3: //Cell is born
				nextGen[row][col] = ALIVE;
				neighbors = 0;
				break;
			default: //Cell dies off
				nextGen[row][col] = DEAD;
				neighbors = 0;
		}	
	}}	
}

//-------------------------------------------------------------------------- 
//Function:     clearOutput() 
// 
// Description: Clears stdout using a Windows system command 
//-------------------------------------------------------------------------- 
void clearOutput(void) 
{ 
    system("CLS"); 
} 
  
//-------------------------------------------------------------------------- 
//Function:     playAgain() 
// 
// Description: Prompts user to quit or play the Game of Life again, takes 
//              in user's choice, then clears the buffer and output from 
//              stdout 
// 
// Input:       A char representing user's choice 
// 
// Output:      A prompt to continue or quit 
//-------------------------------------------------------------------------- 
char playAgain(void) 
{ 
    char userQuit = '\0'; 
  
    printf("\nEnter 'y' to play again or any other key to quit: "); 
    userQuit = getchar(); 
    clearBuffer(); 
    clearOutput(); 
  
    return userQuit; 
} 
  
//-------------------------------------------------------------------------- 
//Function:     clearArrays() 
// 
// Description: Sets all cells in both 2-D arrays to DEAD (a space) 
// 
// Parameters:  char (*pointer1)[LARGE_BOUNDS]: A pointer to the first 2-D
//                                              array. 
//              char (*pointer2)[LARGE_BOUNDS]: A pointer to the second 2-D
//-------------------------------------------------------------------------- 
void clearArrays(char (*pointer1)[LARGE_BOUNDS], 
	char (*pointer2)[LARGE_BOUNDS]) 
{ 
    int i = 0; 
    int j = 0; 
  
    for(i = 0; i < LARGE_BOUNDS; i++) 
    { 
        for(j = 0; j < LARGE_BOUNDS; j++) 
        { 
            pointer1[i][j] = DEAD; 
            pointer2[i][j] = DEAD; 
        } 
    } 
} 
  
//-------------------------------------------------------------------------- 
//Function:     swapPointers() 
// 
// Description: Swaps the address of the two pointer to pointer parameters 
//              so that the pointers in main alternate between the arrays 
//              currentGen and nextGen 
// 
// Parameters:  char (**pointer1)[LARGE_BOUNDS]: A pointer to the address of 
//                                               the first pointer in main() 
//              char (**pointer2)[LARGE_BOUNDS]: A pointer to the address of 
//                                              the second pointer in main() 
//              char (*currentGen)[LARGE_BOUDNS]: A pointer to the array 
//                                                currentGen in main() 
//-------------------------------------------------------------------------- 
void swapPointers(char (**pointer1)[LARGE_BOUNDS],  
    char (**pointer2)[LARGE_BOUNDS], char (*currentGen)[LARGE_BOUNDS]) 
{ 
    char (*tempPointer)[LARGE_BOUNDS] = NULL; 
  
    if(*pointer1 == currentGen) 
    { 
      tempPointer = *pointer1; 
      *pointer1 = *pointer2; 
      *pointer2 = tempPointer; 
    } 
    else
    { 
      tempPointer = *pointer2; 
      *pointer2 = *pointer1; 
      *pointer1 = tempPointer; 
      
    } 
}
//--------------------------------------------------------------------------
//Function:   	autoRun()
//
// Description:	Runs through generations until it detects keyboard input
//
// Parameters:  char (*pointer1)[LARGE_BOUNDS]: pointer to a 2-D array
//
//   			char (*pointer2)[LARGE_BOUNDS] : pointer to a 2-D array
//
//          	char (*currentGen)[LARGE_BOUNDS]:address of currentGen array
//
//          	int rows, int columns: number of rows and columns in the array
//--------------------------------------------------------------------------
void autoRun(char (*pointer1)[LARGE_BOUNDS], char (*pointer2)[LARGE_BOUNDS],
	int rows, int columns, char (*currentGen)[LARGE_BOUNDS])
{
	for(;;)
	{
   		printOutNextGen(pointer1, rows, columns);
    	getNextGen(pointer1, pointer2, columns,  rows);
    	swapPointers(&pointer1 , &pointer2 , currentGen);
    	printf("\n**** Press Enter to switch to manual operation. ****");
    	if(kbhit())
    	{
        	break;
    	}
	}
}

//--------------------------------------------------------------------------
//Function:   	bearWave()
//
// Description: Makes a waving bear
//
// Parameters:  FILE * inputFilename: pointer to the file being read
//
//   			char inputArray1[][LARGE_BOUNDS]: array to hold first file
//
//          	char inputArray2[][LARGE_BOUNDS] : array to hold second file
//
//          	char tempArray[LARGE_BOUNDS][LARGE_BOUNDS] : array to hold 
//									copies of files created by readArray()
//--------------------------------------------------------------------------
void bearWave(FILE * inputFilename, char inputArray1[][LARGE_BOUNDS]
,char inputArray2[][LARGE_BOUNDS], 
	char tempArray[LARGE_BOUNDS][LARGE_BOUNDS])
{
    int i = 0;
	int rows = 30; //Size of bearWave1.txt and bearWave2.txt
	int columns = 40;

	inputFilename = fopen("bearWave1.txt", "r");
    readArray(rows, columns, inputFilename, inputArray1, tempArray);
    fclose(inputFilename);
    inputFilename = fopen("bearWave2.txt", "r");
    readArray(rows, columns, inputFilename, inputArray2, tempArray);
    fclose(inputFilename);
    
	for(i = 0; i < 10; i++) //10 iterations to give a few seconds of waving
    {
       printOutNextGen(inputArray1, rows, columns);
       printOutNextGen(inputArray2, rows, columns);
    }
}