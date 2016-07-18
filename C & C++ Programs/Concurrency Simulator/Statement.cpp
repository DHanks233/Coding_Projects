//-----------------------------------------------------------------------------
// Filename:    Statement.cpp
// Classes:     Statement
//
// Methods:     Statement() Default Constructor
//              getType()
//              getVariableIndex()
//              getTime()
//              getValue()
//              setData()
//-----------------------------------------------------------------------------
#include "Statement.h"

//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: Statement object created with all values set to 0 equivalent
//---------------------------------------------------------------------
Statement::Statement()
{
    type = NONE;
    variableIndex = 0;
    timeRequired = 0;
    variableValue = 0;
}

//---getType-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A StatementType item, the type of Statement
//---------------------------------------------------------------------
StatementType Statement::getType() const
{
    return type;
}

//---getVariableIndex--------------------------------------------------
// Pre:  Statement is of ASSIGNMENT or OUTPUT type
// Post: None
//
// Returns: An int, the array index of the variable to be used
// THROWS: logic_error exception if preconditions are not met
//---------------------------------------------------------------------
int Statement::getVariableIndex() const
{
    //If statement type is lock, unlock or end, that don't require access
    //to the variable
    if(type != ASSIGNMENT && type != OUTPUT)
    {
        throw std::logic_error
            ("Statement type contains no variable to use.");
    }
    return variableIndex;
}

//---getTime-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: An int, the time required for the statement to execute
//---------------------------------------------------------------------
int Statement::getTime() const
{
    return timeRequired;
}

//---getValue----------------------------------------------------------
// Pre:  Statement is of ASSIGNMENT type
// Post: None
//
// Returns: An int, the value of the variable to be used
// THROWS: logic_error exception if preconditions are not met
//---------------------------------------------------------------------
unsigned int Statement::getValue() const
{
    //If statement type is output, lock, unlock or end, that don't require
    //access to the value of the variable
    if(type != ASSIGNMENT)
    {
        throw std::logic_error
            ("Statement type contains no variable value.");
    }
    return variableValue;
}

//---setData-----------------------------------------------------------
// Pre:  A Tokenizer object has used getNextLine() to get all input for
//       a single statement
// Post: All data members are set to values provided by the Tokenizer
//       parameter
//
// Returns: A bool, false if type of statement is NONE, true otherwise
// Parameter:
//      const Tokenizer&: The Tokenizer object to be used to set all
//                        Statement fields using input from cin
//---------------------------------------------------------------------
bool Statement::setData(const Tokenizer & inputData)
{
    type = inputData.getType();
    variableIndex = 0;
    variableValue = 0;
    //Set appropriate data based on the type of statement
    switch(type)
    {
        case ASSIGNMENT:
            timeRequired = inputData.getAssignmentTime();
            //Change the variable from char to an array index
            variableIndex = static_cast<int>(inputData.getVariable()) 
                - static_cast<int>('a');
            variableValue = inputData.getValue();
            break;

        case OUTPUT:
            timeRequired = inputData.getOutputTime();
            //Change the variable from char to an array index
            variableIndex = static_cast<int>(inputData.getVariable()) 
                - static_cast<int>('a');
            break;

        case LOCK:
            timeRequired = inputData.getLockTime();
            break;

        case UNLOCK:
            timeRequired = inputData.getUnlockTime();
            break;

        case END_PROGRAM:
            timeRequired = inputData.getEndTime();
            break;
    }
    //If the type is NONE, statement was not read or input properly
    if(type == NONE)
    {
        return false;
    }
    return true;
}