//-----------------------------------------------------------------------------
//	Filename:		compiledNfaEpsilon.cpp
//	Classes:		compiledNfaEpsilon
//	Methods:		isSetInGoalNodes()
//                  checkFinalStates()
//					currentToNextSet()
//					epsilonClosure()
//					compiledNfaEpsilon()
//					compiledNfaEpsilon(const FiniteStateMachine &)
//					isAcceptedString()
//
//	Written By:		Daniel Hanks
//-----------------------------------------------------------------------------
#include "CompiledNfaEpsilon.h"
#include <stdexcept>

typedef unordered_multimap<string, int>::iterator MMI;

//-----------------------------------------------------------------------------
// Default Constructor
//-----------------------------------------------------------------------------
CompiledNfaEpsilon::CompiledNfaEpsilon()
{
    startNode = 0;
}

//-----------------------------------------------------------------------------
// Constructor w/ FiniteStateMachine parameter
//
// This constructor uses the FiniteStateMachine parameter to construct a
// corresponding NFA-epsilon. Throws invalid_argument exception is the 
// FiniteStateMachine passed in contains no nodes or goalNodes
//-----------------------------------------------------------------------------
CompiledNfaEpsilon::CompiledNfaEpsilon(const FiniteStateMachine& newMachine)
{
	if(newMachine.nodes.empty() || newMachine.goalNodes.empty())
	{
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
//-----------------------------------------------------------------------------
void CompiledNfaEpsilon::insertTransitionsIntoMap(const FiniteStateMachine& newNFA)
{
    for(auto currentTrans = newNFA.transitions.begin(); 
        currentTrans != newNFA.transitions.end(); currentTrans++) {
        string newKey = to_string(currentTrans->source) + currentTrans->transitionChar;
        transitions.insert(make_pair(newKey, currentTrans->destination));
    }
}

//-----------------------------------------------------------------------------
// isAcceptedString
//
// Reads through the string passed as parameter to determine whether that string
// is in the accepted language (returns true) or not (returns false) for the 
// current CompiledNfaEpsilon
//-----------------------------------------------------------------------------
bool CompiledNfaEpsilon::isAcceptedString(const string& inputString)
{
    unordered_set<int> firstNodeSet, nextNodeSet;

    firstNodeSet.insert(startNode);
	epsilonClosure(firstNodeSet, startNode, transitions);
    for(auto currentChar = inputString.begin(); currentChar != inputString.end(); 
        currentChar++) {
        if(!firstNodeSet.empty()) {
            currentToNextSet(firstNodeSet, nextNodeSet, *currentChar);
        }
        else {
            currentToNextSet(nextNodeSet, firstNodeSet, *currentChar);
        }
		//If there are no transitions on the current character
        if(firstNodeSet.empty() && nextNodeSet.empty()) {
            return false;
        }
    }
    return isSetInGoalNodes(firstNodeSet, nextNodeSet);
}

//-----------------------------------------------------------------------------
// isSetInGoalNodes
//
// Returns true if the non-empty set of the two sets passed in as parameters
// contains an NFA node that is in goalNodes
//-----------------------------------------------------------------------------
bool CompiledNfaEpsilon::isSetInGoalNodes(unordered_set<int>& firstNodeSet, 
                      unordered_set<int>& nextNodeSet)
{
    if(!firstNodeSet.empty()) {
        return checkFinalStates(firstNodeSet);
    }
    else {
        return checkFinalStates(nextNodeSet);
    }
}

//-----------------------------------------------------------------------------
// checkFinalStates
//
// Determines whether or not the final NFA states reach on the input string are
// one of the goal states (returns true) or not (returns false).
//-----------------------------------------------------------------------------
bool CompiledNfaEpsilon::checkFinalStates(unordered_set<int>& currentNodeSet)
{
    unordered_set<int>::iterator setItem;

    while(!currentNodeSet.empty()) {
        setItem = currentNodeSet.begin();
        int currentState = *setItem;
        currentNodeSet.erase(currentState);
        if(goalNodes.count(currentState) != 0) {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// currentToNextSet
//
// Takes all the nodes contained in the sourceSet, performs and epsilon closure
// on each and places all resulting nodes into the destinationSet
//-----------------------------------------------------------------------------
void CompiledNfaEpsilon::currentToNextSet(unordered_set<int> &currentNodeSet, 
			unordered_set<int> &nextNodeSet, const char currentChar)
{
	while(!currentNodeSet.empty()) {
		unordered_set<int>::iterator firstItem = currentNodeSet.begin();
		int currentState = *firstItem;
		currentNodeSet.erase(currentState);
		string key = to_string(currentState) + currentChar;
		pair<MMI, MMI> range = transitions.equal_range(key);
		for(MMI currentTransition = range.first; currentTransition != range.second; 
            ++currentTransition) {
			nextNodeSet.insert(currentTransition->second);
			epsilonClosure(nextNodeSet, currentTransition->second, transitions);
		}
	}
}

//-----------------------------------------------------------------------------
// epsilonClosure
//
// Gets each node reachable by epsilon from the currentState and places them
// into the sourceSet.
//-----------------------------------------------------------------------------
void CompiledNfaEpsilon::epsilonClosure(unordered_set<int> &currentNodeSet, 
            const int currentState, unordered_multimap<string, int> &map)
{
	string key = to_string(currentState) + EPSILON;
	pair<MMI, MMI> range = map.equal_range(key);

	for(auto currentTransition = range.first; currentTransition != range.second; 
        currentTransition++) {
		epsilonClosure(currentNodeSet, currentTransition->second, map);
		currentNodeSet.insert(currentTransition->second);
	}
}