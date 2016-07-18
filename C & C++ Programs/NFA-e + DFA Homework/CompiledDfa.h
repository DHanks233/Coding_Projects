//-----------------------------------------------------------------------------
//	Filename:		compiledDfa.h
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
#ifndef COMPILEDDFA_H
#define COMPILEDDFA_H
#include <iostream>
#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <list>
#include <utility>
using namespace std;

//-----------------------------------------------------------------------------
//	Struct:			Transition
//	Description:	A representation of a Transition within a finite automaton
//
//	Data Members:	int source:		The node receiving a transition on a char
//					char transitionChar: The character representing a transition
//					int destination: The node of the transition
//-----------------------------------------------------------------------------
struct Transition
{
    int source;
    char transitionChar;
    int destination;

    Transition() : source(0), transitionChar('\0'), destination(0) {}
    Transition(int start, char transition, int end) : source(start), 
        transitionChar(transition), destination(end) {}
};

//-----------------------------------------------------------------------------
//	Struct:			FiniteStateMachine
//	Description:	A representation of a single finite state machine
//
//	Data Members:	unordered_set<int> nodes: All nodes in the DFA
//					int startNode: The state all transitions begin from
//					unordered_set<int> goalNodes: All accepting states
//					list<Transition>: All transitions from all nodes
//-----------------------------------------------------------------------------
struct FiniteStateMachine
{
    unordered_set<int> nodes;
    int startNode;
    unordered_set<int> goalNodes;
    list<Transition> transitions;

    FiniteStateMachine() : startNode(0) {}
    FiniteStateMachine(unordered_set<int> allNodes, int start, 
		unordered_set<int> goal, list<Transition> trans) :
        nodes(allNodes), startNode(start), goalNodes(goal), transitions(trans) {}
};

//-----------------------------------------------------------------------------
//	Class:			compiledDfa
//	Description:	The compiledDfa class takes a FiniteStateMachine struct and
//					builds an internal representation of a Deterministic 
//					Finite Automaton which can then be used to evaluate string
//					objects.
//
//	Data Members:	unordered_set<int> nodes: All nodes in the DFA
//					int startNode: The state all transitions begin from
//					unordered_set<int> goalNodes: All accepting states
//					unordered_map<string, int>: All transitions from all nodes
//-----------------------------------------------------------------------------
class CompiledDfa
{
    private:
        unordered_set<int> nodes;
        int startNode;
        unordered_set<int> goalNodes;
        unordered_map<string, int> transitions;

        void insertTransitionsIntoMap(const FiniteStateMachine& newMachine);

    public:
        //-----------------------------------------------------------------------------
        // Default Constructor
        //
        // Uses the STL object constructors to initilize sets and maps, and sets
        // startNode to 0.
        //-----------------------------------------------------------------------------
        CompiledDfa();

        //-----------------------------------------------------------------------------
        // Constructor w/ FiniteStateMachine parameter
        //
        // This constructor copies the information from the FiniteStateMachine passed
        // in into the compiledDfa object.
        //
        // Throws invalid_argument exception if the FiniteStateMachine passed as
        // parameter contains no nodes or no goalNodes
        //-----------------------------------------------------------------------------
        CompiledDfa(const FiniteStateMachine& newMachine);

        //-----------------------------------------------------------------------------
        // isAcceptedString
        //
        // Reads through the string passed as parameter to determine whether that string
        // is in the accepted language (returns true) or not (returns false) for the
        // current compiledDfa.
        //-----------------------------------------------------------------------------
        bool isAcceptedString(const string& inputString);
};

#endif