//-----------------------------------------------------------------------------
// Filename:    Tokenizer.cpp
// Classes:     Tokenizer
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
#include "Tokenizer.h"

//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: Tokenizer object created with all values set to 0 equivalent
//---------------------------------------------------------------------
Tokenizer::Tokenizer()
{
        numberPrograms = 0;
        timeQuantum= 0;
        assignmentTime = 0;
        outputTime = 0;
        lockTime = 0;
        unlockTime = 0;
        endTime = 0;
        variable = 0;
        variableValue = '\0';
        type = NONE;
}

//---getNumberPrograms-------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the number of programs to be created
//---------------------------------------------------------------------
int Tokenizer::getNumberPrograms() const
{
    return numberPrograms;
}

//---getTimeQuantum----------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time quantum alloted for each programs'
//          execution
//---------------------------------------------------------------------
int Tokenizer::getTimeQuantum() const
{
    return timeQuantum;
}

//---getAssignmentTime-------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time taken to execute an assignment statement
//---------------------------------------------------------------------
int Tokenizer::getAssignmentTime() const
{
    return assignmentTime;
}

//---getOutputTime-----------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time taken to execute an output statement
//---------------------------------------------------------------------
int Tokenizer::getOutputTime() const
{
    return outputTime;
}

//---getLockTime-------------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time taken to execute an lock statement
//---------------------------------------------------------------------
int Tokenizer::getLockTime() const
{
    return lockTime;
}

//---getUnlockTime-----------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time taken to execute an unlock statement
//---------------------------------------------------------------------
int Tokenizer::getUnlockTime() const
{
    return unlockTime;
}

//---getEndTime--------------------------------------------------------
// Pre:  Tokenizer has used getFirstLine() to read in parameters
// Post: None
//
// Returns: An int, the time taken to execute an end statement
//---------------------------------------------------------------------
int Tokenizer::getEndTime() const
{
    return endTime;
}

//---getVariable-------------------------------------------------------
// Pre:  Tokenizer has used getNextLine() to read in a statement, and
//       the type of statement is ASSIGNMENT or OUTPUT
// Post: None
//
// Returns: A char, represents the variable name to be used, 'a' to 'z'
//---------------------------------------------------------------------
char Tokenizer::getVariable() const
{
    return variable;
}

//---getValue----------------------------------------------------------
// Pre:  Tokenizer has used getNextLine() to read in a statement, and
//       the statement type is ASSIGNMENT
// Post: None
//
// Returns: An int, the value of the variable to be used
//---------------------------------------------------------------------
unsigned int Tokenizer::getValue() const
{
    return variableValue;
}

//---getType-----------------------------------------------------------
// Pre:  Tokenizer has used getNextLine() to read in a statement
// Post: None
//
// Returns: A StatementType item, the type of statement
//---------------------------------------------------------------------
StatementType Tokenizer::getType() const
{
    return type;
}

//---getFirstLine------------------------------------------------------
// Pre:  The list contains at least one item
// Post: All data members associated with the entire simulator are set,
//       namely numberPrograms, timeQuantum and all five times
//       associated with five statement types
//
// Returns: A bool that is false if any of the data values read in
//          are invalid (<1), true if all input is valid.
//---------------------------------------------------------------------
bool Tokenizer::getFirstLine()
{
    vector<int> commands; //Used to check if any invalid input is entered

        cin >> numberPrograms;              //Get each input in turn
        commands.push_back(numberPrograms); //Push each input onto vector
        cin >> assignmentTime;
        commands.push_back(assignmentTime);
        cin >> outputTime;
        commands.push_back(outputTime);
        cin >> lockTime;
        commands.push_back(lockTime);
        cin >> unlockTime;
        commands.push_back(unlockTime);
        cin >> endTime;
        commands.push_back(endTime);
        cin >> timeQuantum;
        commands.push_back(timeQuantum);
        cin.ignore(MAX_READ, END_OF_LINE);
        for(unsigned int i = 0; i < commands.size(); i++)
        {
            if(commands[i] < 1)
            {
                //If one of the input parameters is invalid, return false
                return false;
            }
        }
        return true;
}

//---getNextLine-------------------------------------------------------
// Pre:  The list contains at least one item
// Post: All data members associated with a statement are set, namely
//       type and possibly variableValue and variable depending on type
//
// Returns: A bool that is false if any variable or variable value read
//          from input is invalid. Variable must be 'a' to 'z',
//          variable value from 0 to 100.
//---------------------------------------------------------------------
bool Tokenizer::getNextLine()
{
    stringstream ss;
    string command("");

    getline(cin, command, END_OF_LINE); //Read next line of input from cin
    ss << command;
    ss >> command; //Use stringstream to eliminate whitespace

    //Assure all commands are in lower case, so they can be compared properly
    for(string::iterator it = command.begin(); it != command.end(); it++)
    {
        *it = tolower(*it);
    }

    //Act according to the type of statement read in
    if(command == "print")
    {
        type = OUTPUT;
        ss >> variable;
        //If variable input is invalid, return false
        if(variable < 'a' || variable > 'z')
        {
            return false;
        }
    }
    else if(command == "lock")
    {
        type = LOCK;
    }
    else if(command == "unlock")
    {
        type = UNLOCK;
    }
    else if(command == "end")
    {
        type = END_PROGRAM;
    }
    else
    {
        type = ASSIGNMENT;
        //If there was no whitespace between operators and operand
        if(command.length() > 1)
        {
            ss.clear();
            ss << command;
            ss >> variable;
            ss.ignore(MAX_READ, '=');
            ss >> variableValue;
        }
        //If input has whitespace betwen operators and operand
        else
        {
            ss.ignore(MAX_READ, '=');
            ss >> variableValue;
            ss.clear();
            ss << command;
            ss >> variable;
        }
        //If variable or the variable's value input is invalid, return false
        if((variable < 'a' || variable > 'z') ||
            (variableValue < 0 || variableValue > 100))
        {
            return false;
        }
    }
    return true;
}