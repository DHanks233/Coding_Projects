//-----------------------------------------------------------------------------
// Filename:    Program.h
// Classes:     Program
//
// Methods:     Program() Default Constructor
//              Program() Constructor #2
//              isEmpty()
//              addStatement()
//              removeStatement()
//              getIsBlocked()
//              getLockActive()
//              setIdent()
//              unlockProgram()
//              executeStatement()
//              peek()
//              clear()
//
// Other Functions: runSimulation()
//-----------------------------------------------------------------------------
#ifndef PROGRAM_H
#define PROGRAM_H
#include <iostream>
#include "LListQueue.h"
#include "Statement.h"
using namespace std;

const int MAX_STORAGE = 26;

//-----------------------------------------------------------------------------
// Program class:
// An object of this class represents a single program, which is made up of a
// list of statements queued in the correct order to be executed. There are
// also 3 static private members to provide shared access among all Program 
// objects.
//
// An SLinkedList object contains six private data members:
//
//      LListQueue<Statement> statementList: The queue of Statements to be
//                          executed in the simulation
//
//      int programIdent:   The unique numerical identifier for each program
//                          object to differentiate them during output
//
//      bool isLocked:      True if the program is in a lock state, false
//                          otherwise
//
//      static int timeQuantum: The amount of time alloted for a single
//                          program's control of data and output
//
//      static bool lockActive: True if one program has executed a lock
//                          statement and no unlock, false otherwise
//
//      static int sharedStorage[26]: The shared variable storage for all
//                          program objects. Represents variables 'a' to 'z'
//
// Implementation and assumptions:
//   -- All static data members are used to provide shared access for all
//      program objects instantiated
//   -- programIdent will begin at '1' for the first program, '2' for the
//      second program and so on
//   -- programIdent will be used on output to differentiate which program is
//      actually printing values
//   -- If one program executes a lock statement, lockActive is set to true
//   -- If lockActive is true and another program attempts to execute a lock
//      statement, isLocked is set to true for the object trying to execute 
//      the lock statement
//   -- isEmpty() checks only that statementList is empty
//   -- executeStatement() takes action depending on the Statement type, then
//      uses removeStatement() to dequeue that statement from the list
//   -- If a Program attempts to execute a lock Statement but lockActive is
//      true, that lock Statement is not executed or dequeued
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------

class Program
{
    private:

        LListQueue<Statement> statementList;    //Queue of all Statements
        int programIdent;                       //Program identifier
        bool isBlocked;                          //Is program locked or unlocked
        static int timeQuantum;                 //Time for program execution
        static bool lockActive;                 //If a lock is active
        static int sharedStorage[MAX_STORAGE];  //Shared variable storage

    public:

        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: Statement object created with all values set to 0 equivalent
        //---------------------------------------------------------------------
        Program();

        //---Constructor w/ Time + Ident---------------------------------------
        // Pre:  None
        // Post: Statement object created with all values set to 0 equivalent,
        //       programIdent assigned and timeQuantum assigned if it has not
        //       been already
        //
        // Parameters:
        //      int newTime: The time quantum to be used for all Programs
        //      int newNumber: The identifier for the Program being created
        //---------------------------------------------------------------------
        Program(int newTime, int newNumber);

        //---isEmpty-----------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: A bool that is true if the Statement queue is empty, false
        //          otherwise
        //---------------------------------------------------------------------
        bool isEmpty() const;

        //---addStatement------------------------------------------------------
        // Pre:  None
        // Post: The Statement passed as parameter is appended to 
        //       statementList.
        //
        // Returns: A bool that is true if the Statement is successfully added
        //          to the queue, false otherwise
        // Parameter:
        //      const Statement &: The Statement to be added to statementList
        //---------------------------------------------------------------------
        bool addStatement(const Statement & );

        //---removeStatement---------------------------------------------------
        // Pre:  statementList contains at least one Statement
        // Post: The first Statement in the list is dequeued
        //
        // Returns: A bool that is true if the Statement is successfully added
        //          to the queue, false otherwise
        //---------------------------------------------------------------------
        bool removeStatement();

        //---getIsBlocked-------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: A bool of data member isLocked, representing whether or not
        //          the Program object is blocked
        //---------------------------------------------------------------------
        bool getIsBlocked() const;

        //---getLockActive-----------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: A bool of static data member lockActive, representing
        //          whether one Program has executed a lock statement but no
        //          unlock statement as of yet
        //---------------------------------------------------------------------
        bool getLockActive() const;

        //---setIdent----------------------------------------------------------
        // Pre:  None
        // Post: programIdent is set to provide the Program object with a
        //       unique numerical identifier
        //
        // Parameter:
        //      int: The value which programIdent is set to
        //---------------------------------------------------------------------
        void setIdent(int);

        //---unlockProgram-----------------------------------------------------
        // Pre:  lockActive == true
        // Post: lockActive == false
        //
        // Returns: A bool that is false if lockActive is already false,
        //          meaning the program is not locked, true if lockActive is
        //          successfully changed from true to false.
        //---------------------------------------------------------------------
        bool unlockProgram();

        //---executeStatement--------------------------------------------------
        // Pre:  statementList contains at least one Statement
        // Post: The first Statement in the queue is executed based on the type
        //       of Statement, then that Statement is dequeued
        //
        // Returns: A bool that is false if statementList is empty before
        //          execution, true otherwise
        // Parameters:
        //      LListQueue<Program>& activeQueue: The queue of Programs
        //                                        currently executing
        //      LListQueue<Program>& blockedQueue: Queue of Programs currently
        //                                         suspended due to a lock
        //---------------------------------------------------------------------
        bool executeStatement(LListQueue<Program>& activeQueue,
                                LListQueue<Program>& blockedQueue);

        //---peek--------------------------------------------------------------
        // Pre:  statementList contains at least one Statement
        // Post: None
        //
        // Returns: A reference of the first Statement in the statementList
        //
        // THROWS:  out_of_range error if statementList is empty
        //---------------------------------------------------------------------
        Statement peek() const;

        //---clear-------------------------------------------------------------
        // Pre:  None
        // Post: All Statements in statementList are properly deleted
        //
        // Returns: A bool that is true if statementList is empty, false 
        //          otherwise
        //---------------------------------------------------------------------
        bool clear();
};

//---runSimulation-------------------------------------------------------------
// Pre: Parameter activeQueue contains at least one Program object
// Post: All Statements from the statementList data member of every Program
//       object in activeQueue are executed and dequeued. activeQueue is empty.
//
// Parameter:
//      LListQueue<Program>& activeQueue: A queue of Program objects to be 
//          executed until all statementLists are empty
//-----------------------------------------------------------------------------
void runSimulation(LListQueue<Program>& );

#endif