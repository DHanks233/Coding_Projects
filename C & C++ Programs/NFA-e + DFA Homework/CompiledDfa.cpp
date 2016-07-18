//-----------------------------------------------------------------------------
//	Filename:		compiledDfa.cpp
//	Classes:		compiledDfa
//	Methods:		compiledDfa()
//					compiledDfa(const FiniteStateMachine &)
//                  insertTransitionsIntoMap()
//					isAcceptedString()
//
//	Structs:		Transition
//					FiniteStateMachine
//
//	Written By:		Daniel Hanks
//-----------------------------------------------------------------------------
#include "CompiledDfa.h"
#include <stdexcept>
using namespace std;

//-----------------------------------------------------------------------------
// Default Constructor
//-----------------------------------------------------------------------------
CompiledDfa::CompiledDfa()
{
    startNode = 0;
}

//-----------------------------------------------------------------------------
// Constructor w/ FiniteStateMachine parameter
//
// This constructor copies the information from the FiniteStateMachine passed
// in into the compiledDfa object.
//
// Throws invalid_argument exception if the FiniteStateMachine passed as
// parameter contains no nodes or no goalNodes
//-----------------------------------------------------------------------------
CompiledDfa::CompiledDfa(const FiniteStateMachine& newMachine)
{

	if(newMachine.nodes.empty() || newMachine.goalNodes.empty()) {
		throw invalid_argument("Finite State Machine is not valid.");
	}
    nodes = newMachine.nodes;
    startNode = newMachine.startNode;
    goalNodes = newMachine.goalNodes;
    insertTransitionsIntoMap(newMachine);
}

//-----------------------------------------------------------------------------
// insertTransitionsIntoMap
//
// Takes all the transitions from the FiniteStateMachine& parameter and inserts
// them into an unordered_map<string, int> for O(1) access.
//
// Throws out_of_range exception if there are multiple transitions on the same
// character from the same node, violating an invariant of a DFA
//-----------------------------------------------------------------------------
void CompiledDfa::insertTransitionsIntoMap(const FiniteStateMachine& newMachine)
{
    for(auto currentTrans : newMachine.transitions)
    {
        string newKey = to_string(currentTrans.source) + currentTrans.transitionChar;
		if(transitions.count(newKey) != 0) {
			throw out_of_range("A DFA cannot have multiple transitions on "
				"the same character from the same state.");
		}
        transitions.insert(make_pair(newKey, currentTrans.destination));
    }
}

//-----------------------------------------------------------------------------
// isAcceptedString
//
// Reads through the string passed as parameter to determine whether that string
// is in the accepted language (returns true) or not (returns false) for the
// current compiledDfa.
//-----------------------------------------------------------------------------
bool CompiledDfa::isAcceptedString(const string& inputString)
{
    int currentState = startNode;

    for(auto currentChar : inputString) {
        string key = to_string(currentState) + currentChar;
        if(transitions.count(key) == 0) {
            return false;
        }
        currentState = transitions.find(key)->second;
    }
    return (goalNodes.count(currentState) != 0);
}