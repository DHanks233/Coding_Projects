//-----------------------------------------------------------------------------
// Filename:    Program.cpp
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
#include "Program.h"

//Declare and initialize static data members
bool Program::lockActive = false;
int Program::timeQuantum = 0;
int Program::sharedStorage[MAX_STORAGE] = {0};

//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: Statement object created with all values set to 0 equivalent
//---------------------------------------------------------------------
Program::Program()
{
    programIdent = 0;
    isBlocked = false;
}

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
Program::Program(int newTime, int newNumber)
    : programIdent(newNumber)
{
    isBlocked = false;
    if(newNumber == 1)
    {
        timeQuantum = newTime;
    }
}

//---isEmpty-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A bool that is true if the Statement queue is empty, false
//          otherwise
//---------------------------------------------------------------------
bool Program::isEmpty() const
{
    return statementList.isEmpty();
}

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
bool Program::addStatement(const Statement & newStatement)
{
    return statementList.enqueue(newStatement);
}

//---removeStatement---------------------------------------------------
// Pre:  statementList contains at least one Statement
// Post: The first Statement in the list is dequeued
//
// Returns: A bool that is true if the Statement is successfully added
//          to the queue, false otherwise
//---------------------------------------------------------------------
bool Program::removeStatement()
{
    return statementList.dequeue();
}

//---getIsBlocked-------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A bool of data member isBlocked, representing whether or not
//          the Program object is blocked
//---------------------------------------------------------------------
bool Program::getIsBlocked() const
{
    return isBlocked;
}

//---getLockActive-----------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A bool of static data member lockActive, representing
//          whether one Program has executed a lock statement but no
//          unlock statement as of yet
//---------------------------------------------------------------------
bool Program::getLockActive() const
{
    return lockActive;
}

//---setIdent----------------------------------------------------------
// Pre:  None
// Post: programIdent is set to provide the Program object with a
//       unique numerical identifier
//
// Parameter:
//      int: The value which programIdent is set to
//---------------------------------------------------------------------
void Program::setIdent(int newNumber)
{
    programIdent = newNumber;
}

//---executeStatement--------------------------------------------------
// Pre:  lockActive == true
// Post: The Program object has lockActive set to false
//
// Returns: A bool that is false if lockActive is already false, meaning
//          the program is not locked, true if lockActive is successfully
//          changed from true to false.
//---------------------------------------------------------------------
bool Program::unlockProgram()
{
    if(isBlocked == false)
    {
        return false;
    }
    else
    {
        isBlocked = false;
        return true;
    }
}

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
bool Program::executeStatement(LListQueue<Program>& activeQueue,
                               LListQueue<Program>& blockedQueue)
{
    Statement tempStat;             //To hold temporary Statement
    Program tempProgram;            //In case of unlock Statement
    int tempQuantum = timeQuantum;

    if(isEmpty())
    {
        return false;
    }
    do
    {
        //Get first item in statementList
        tempStat = statementList.peek();
        switch(tempStat.getType())
        {
            case ASSIGNMENT:
                sharedStorage[tempStat.getVariableIndex()] 
                = tempStat.getValue();
                break;

            case OUTPUT:
                cout << programIdent << ": " 
                    << sharedStorage[tempStat.getVariableIndex()] << endl;
                break;

            case LOCK:
                //If another Program has not executed lock
                if(lockActive == false)
                {
                    lockActive = true;
                }
                //If another Program has executed a lock, block this Program
                else
                {
                    isBlocked = true;
                }
                break;

            case UNLOCK:
                //If an unlock command is executed, get the first Program out
                //of the blocked queue, put it into the activeQueue immediately
                if(!blockedQueue.isEmpty())
                {
                    tempProgram = blockedQueue.peek();
                    tempProgram.unlockProgram();
                    blockedQueue.dequeue();
                    activeQueue.enqueue(tempProgram);
                }
                lockActive = false;
                break;

            //If executing an end Statement, make sure statementList is empty
            default:
                statementList.clear();
        }
        //Do not dequeue the lock Statement for a Program being blocked
        if(!isBlocked)
        {
            statementList.dequeue();
        }
    } while((tempQuantum -= tempStat.getTime()) > 0 
        && !(statementList.isEmpty()));
    
    tempProgram.clear();
    return true;
}

//---peek--------------------------------------------------------------
// Pre:  statementList contains at least one Statement
// Post: None
//
// Returns: A reference to the first Statement in the statementList
//
// THROWS:  out_of_range error if statementList is empty
//---------------------------------------------------------------------
Statement Program::peek() const
{
    return statementList.peek();
}

//---clear-------------------------------------------------------------
// Pre:  None
// Post: All Statements in statementList are properly deleted
//
// Returns: A bool that is true if statementList is empty, false 
//          otherwise
//---------------------------------------------------------------------
bool Program::clear()
{
    return (statementList.clear());
}

//---runSimulation-------------------------------------------------------------
// Pre: Parameter activeQueue contains at least one Program object
// Post: All Statements from the statementList data member of every Program
//       object in activeQueue are executed and dequeued. activeQueue is empty.
//
// Parameter:
//      LListQueue<Program>& activeQueue: A queue of Program objects to be 
//          executed until all statementLists are empty
//-----------------------------------------------------------------------------
void runSimulation(LListQueue<Program>& activeQueue)
{
    LListQueue<Program> blockedQueue;
    Program nowExecuting;

    while(!activeQueue.isEmpty())
    {
        //Get the first Program in activeQueue and execute its first Statement
        nowExecuting = activeQueue.peek();
        activeQueue.dequeue();
        nowExecuting.executeStatement(activeQueue, blockedQueue);

        //If the Program attempted to execute lock, but another Program has a
        //lock already in place, add to blockedQueue until unlock is executed
        if(nowExecuting.getIsBlocked())
        {
            blockedQueue.enqueue(nowExecuting);
        }
        //If the Program has no more Statements to execute, do not requeue it
        else if(!nowExecuting.isEmpty())
        {
            activeQueue.enqueue(nowExecuting);
        }
        nowExecuting.clear();
    }
}