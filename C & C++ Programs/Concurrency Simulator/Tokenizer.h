//-----------------------------------------------------------------------------
// Filename:    Tokenizer.h
// Classes:     Tokenizer
// Enum:        StatementType
//
// Methods:     Tokenizer(); Default Constructor
//              getNumberPrograms();
//              getTimeQuantum();
//              getAssignmentTime();
//              getOutputTime();
//              getLockTime();
//              getUnlockTime();
//              getEndTime();
//              getVariable();
//              getValue();
//              getType();
//              getFirstLine();
//              getNextLine();
//-----------------------------------------------------------------------------
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;

const int MAX_READ = 256;       //Max number to read using cin.ignore()
const char END_OF_LINE = '\n';

//---StatementType enum--------------------------------------------------------
// Each statement that may be executed has one of 5 types. The enumerated type
// is simply to enhance readability and allow use of switch() statements.
//
// Written by: Daniel Hanks (SID: 0024879)
//-----------------------------------------------------------------------------
enum StatementType
{
    NONE = 0,
    ASSIGNMENT = 1,
    OUTPUT = 2,
    LOCK = 3,
    UNLOCK = 4,
    END_PROGRAM = 5
};

//-----------------------------------------------------------------------------
// Tokenizer class:
// This is a class that is used to parse the input provided for the concurrency
// simulator, including both information used for the whole program, and for
// individual statements.
//
// A Tokenizer object contains 10 private data members:
//
//  int numberPrograms: The number of programs to be created for the simulation
//
//  int timeQuantum: The time given each program to execute its statements
//
//  int assignmentTime: Time required to execute an assignment statement
//
//  int outputTime: Time required to execute an output/print statement
//
//  int lockTime: Time required to execute a lock statement
//
//  int unlockTime: Time required to execute an unlock statement
//
//  int endTime: Time required to execute an end statement
//
//  char variable: The char holding the variable to use, from 'a' to 'z'
//
//  unsigned int variableValue: Value of the variable in the statement
//
//  StatementType type: One of the five types of statements, using enum
//
// Implementation and assumptions:
//   -- Tokenizer objects read input using cin, not from a file
//   -- The getFirstLine() method gets the first line of input which assigns
//      the number of programs, time required to do each type of statement and
//      the time quantum.
//   -- getNextLine() method will execute getting all statements from cin
//      until end of input
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------

class Tokenizer
{
    private:
        int numberPrograms;             //Number of programs created
        int timeQuantum;                //Time alloted each program
        int assignmentTime;             //Time to complete an assignment
        int outputTime;                 //Time to complete an output
        int lockTime;                   //Time to complete a lock
        int unlockTime;                 //Time to complete an unlock
        int endTime;                    //Time to complete program end
        char variable;                  //Variable accessed or assigned
        unsigned int variableValue;     //Value of variable
        StatementType type;             //Type of statement (enum)

    public:

        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: Tokenizer object created with all values set to 0 equivalent
        //---------------------------------------------------------------------
        Tokenizer();

        //---getNumberPrograms-------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the number of programs to be created
        //---------------------------------------------------------------------
        int getNumberPrograms() const;

        //---getTimeQuantum----------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time quantum alloted for each programs'
        //          execution
        //---------------------------------------------------------------------
        int getTimeQuantum() const;

        //---getAssignmentTime-------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time taken to execute an assignment statement
        //---------------------------------------------------------------------
        int getAssignmentTime() const;

        //---getOutputTime-----------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time taken to execute an output statement
        //---------------------------------------------------------------------
        int getOutputTime() const;

        //---getLockTime-------------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time taken to execute an lock statement
        //---------------------------------------------------------------------
        int getLockTime() const;

        //---getUnlockTime-----------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time taken to execute an unlock statement
        //---------------------------------------------------------------------
        int getUnlockTime() const;

        //---getEndTime--------------------------------------------------------
        // Pre:  Tokenizer has used getFirstLine() to read in parameters
        // Post: None
        //
        // Returns: An int, the time taken to execute an end statement
        //---------------------------------------------------------------------
        int getEndTime() const;

        //---getVariable-------------------------------------------------------
        // Pre:  Tokenizer has used getNextLine() to read in a statement, and
        //       the type of statement is ASSIGNMENT or OUTPUT
        // Post: None
        //
        // Returns: A char, represents the variable name to be used, 'a' to 'z'
        //---------------------------------------------------------------------
        char getVariable() const;

        //---getValue----------------------------------------------------------
        // Pre:  Tokenizer has used getNextLine() to read in a statement, and
        //       the statement type is ASSIGNMENT
        // Post: None
        //
        // Returns: An int, the value of the variable to be used
        //---------------------------------------------------------------------
        unsigned int getValue() const;

        //---getType-----------------------------------------------------------
        // Pre:  Tokenizer has used getNextLine() to read in a statement
        // Post: None
        //
        // Returns: A StatementType item, the type of statement
        //---------------------------------------------------------------------
        StatementType getType() const;

        //---getFirstLine------------------------------------------------------
        // Pre:  The list contains at least one item
        // Post: All data members associated with the entire simulator are set,
        //       namely numberPrograms, timeQuantum and all five times
        //       associated with five statement types
        //
        // Returns: A bool that is false if any of the data values read in
        //          are invalid (<1), true if all input is valid.
        //---------------------------------------------------------------------
        bool getFirstLine();

        //---getNextLine-------------------------------------------------------
        // Pre:  The list contains at least one item
        // Post: All data members associated with a statement are set, namely
        //       type and possibly variableValue and variable depending on type
        //
        // Returns: A bool that is false if any variable or variable value read
        //          from input is invalid. Variable must be 'a' to 'z',
        //          variable value from 0 to 100.
        //---------------------------------------------------------------------
        bool getNextLine();
};

#endif