//-----------------------------------------------------------------------------
// Filename:    Statement.h
// Classes:     Statement
//
// Methods:     Statement() Default Constructor
//              getType()
//              getVariableIndex()
//              getTime()
//              getValue()
//              setData()
//-----------------------------------------------------------------------------
#ifndef STATEMENT_H
#define STATEMENT_H
#include "Tokenizer.h"
#include <stdexcept>
using namespace std;

//-----------------------------------------------------------------------------
// Statement class:
// This is a class which holds all data for a single statement to be
// executed by a program, as well as accessors to get that statement's data.
//
// An Statement object contains four private data members:
//
//      StatementType type: The type of statement, one of 5 types represented
//                          by an enum of StatementType
//
//      int variableIndex:  The array index of the variable used
//
//      int timeRequired:   Time required to execute the statement
//
//      unsigned int variableValue: Value of the variable used
//
// Implementation and assumptions:
//   -- All data members are accessible via public methods to allow the program
//      to get all information needed to execute the statement
//   -- The setData() method takes in a Tokenizer object and uses that
//      that parameter to set all data fields for the Statement
//   -- StatementType is an enumerated type of six values:
//          0 = NONE, 1 = ASSIGNMENT, 2 = OUTPUT, 3 = LOCK, 4 = UNLOCK
//          5 = END_PROGRAM
//      And represents one of the five types of statement to be executed
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------

class Statement
{
    private:

        StatementType type;         //Type of the Statement
        int variableIndex;          //Array index of variable
        int timeRequired;           //Time for Statement to execute
        unsigned int variableValue; //Value of variable

    public:

        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: Statement object created with all values set to 0 equivalent
        //---------------------------------------------------------------------
        Statement();

        //---getType-----------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: A StatementType item, the type of Statement
        //---------------------------------------------------------------------
        StatementType getType() const;

        //---getVariableIndex--------------------------------------------------
        // Pre:  Statement is of ASSIGNMENT or OUTPUT type
        // Post: None
        //
        // Returns: An int, the array index of the variable to be used
        // THROWS: logic_error exception if preconditions are not met
        //---------------------------------------------------------------------
        int getVariableIndex() const;

        //---getTime-----------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: An int, the time required for the statement to execute
        //---------------------------------------------------------------------
        int getTime() const;

        //---getValue----------------------------------------------------------
        // Pre:  Statement is of ASSIGNMENT type
        // Post: None
        //
        // Returns: An int, the value of the variable to be used
        // THROWS: logic_error exception if preconditions are not met
        //---------------------------------------------------------------------
        unsigned int getValue() const;

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
        bool setData(const Tokenizer & );
};

#endif