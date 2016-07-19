//-----------------------------------------------------------------------------
// Filename:    lab5_driver.cpp
// 
// Functions:   main()
//
// Written by:  Daniel P. Hanks (SID: 0024879)
//-----------------------------------------------------------------------------
#include "SLinkedList.h"
#include "Node.h"
#include "LListQueue.h"
#include "Tokenizer.h"
#include "Statement.h"
#include "Program.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//---main----------------------------------------------------------------------
// Pre:  None
// Post: Formatted data is entered via cin & read in using a Tokenizer object.
//       Parameters for the simulation are set, and each Statement is read in
//       and stored in its corresponding Program object, until all Programs
//       and their Statements are instantiated and added into a queue
//       (activeQueue). runSimulation() is called to execute all Statements
//       in each Program object.
//
// Returns: An int, 0 if program is successful, 1 otherwise
//-----------------------------------------------------------------------------
int main()
{
    int identifier = 1;    //Provides unique identifier for each Program object
    bool validInput = true;//Used to determine if all input entered is valid
    Tokenizer inputData;   //Takes in all input from cin
    Statement statement;   //Used to build a queue of Statements objects
    Program * newProgram = NULL;  //Used to build a queue of Program objects
    LListQueue<Program> activeQueue; //Queue of Programs to execute

    validInput = inputData.getFirstLine();
    //If invalid input is read, inform user and terminate program with failure
    if(!validInput)
    {
        cout << "All values in the first line must be >= 1" << endl;
        return 1;
    }
    for(int i = 0; i < inputData.getNumberPrograms(); i++)
    {
        newProgram = new Program(inputData.getTimeQuantum(), identifier++);
        do
        {
            validInput = inputData.getNextLine();
            //If variable or variable value input is invalid, terminate program
            if(!validInput)
            {
                cout << "Variables may only be 'a' to 'z' and variable values"
                    " from 0 to 100." << endl;
                return 1;
            }
            statement.setData(inputData);
            newProgram->addStatement(statement);
        } while(statement.getType() != END_PROGRAM);

        activeQueue.enqueue(*newProgram);
        if(newProgram != NULL)
        {
            delete newProgram;
        }
    }
    newProgram = NULL;
    //Now that all Programs and their Statements are created, begin simulation
    runSimulation(activeQueue);

    getchar();
    return 0;
}