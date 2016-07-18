//-----------------------------------------------------------------------------
//	Filename:		compiledNfaEpsilon.h
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
#ifndef COMPILEDNFAEPSILON_H
#define COMPILEDNFAEPSILON_H
#include <iostream>
#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <list>
#include <utility>
#include <algorithm>
#include <functional>
#include <queue>
#include "CompiledDfa.h"
const char EPSILON = 0;
using namespace std;

//Adding a specialization for the std::hash() that takes unordered_set<int> as key
namespace std {
    template <>
        class hash<unordered_set<int>>{
        public :
            size_t operator()(const unordered_set<int> &set ) const
            {
                size_t hashValue = 0;
                for(auto setChar = set.cbegin(); setChar != set.cend(); setChar++)
                {
                    if(*setChar == 0) { //Since '0 xor value = value', hash 0 as -255
                        hashValue = hashValue ^ hash<int>()(-255);
                    }
                    else {
                        hashValue = hashValue ^ hash<int>()(*setChar);
                    }
                }
                return hashValue;
            }
    };
};

//-----------------------------------------------------------------------------
//	Class:			compiledNfa
//	Description:	The compiledNfa class takes a FiniteStateMachine struct and
//					builds an internal representation of a Non-Deterministic 
//					Finite Automaton which can then be used to evaluate string
//					objects.
//
//	Data Members:	unordered_set<int> nodes: All nodes in the NFA
//					int startNode: The state all transitions begin from
//					unordered_set<int> goalNodes: All accepting states
//					unordered_multimap<string, int>: All transitions from all nodes
//-----------------------------------------------------------------------------
class CompiledNfaEpsilon
{
    private:
        unordered_set<int> nodes;
        int startNode;
        unordered_set<int> goalNodes;
        unordered_multimap<string, int> transitions;

        void insertTransitionsIntoMap(const FiniteStateMachine& newNFA);
        bool isSetInGoalNodes(unordered_set<int>& firstNodeSet, 
            unordered_set<int>& nextNodeSet);
		bool checkFinalStates(unordered_set<int>& set);
		void currentToNextSet(unordered_set<int> &sourceSet, 
			unordered_set<int> &destinationSet, const char currentChar);

    public:

        //-----------------------------------------------------------------------------
        // Default Constructor
        //
        // Uses the STL object constructors to initilize sets and maps, and sets
        // startNode to 0.
        //-----------------------------------------------------------------------------
        CompiledNfaEpsilon();

        //-----------------------------------------------------------------------------
        // Constructor w/ FiniteStateMachine parameter
        //
        // This constructor uses the FiniteStateMachine parameter to construct a
        // corresponding NFA-epsilon. Throws invalid_argument exception is the 
        // FiniteStateMachine passed in contains no nodes or goalNodes
        //-----------------------------------------------------------------------------
        CompiledNfaEpsilon(const FiniteStateMachine& newMachine);

        //-----------------------------------------------------------------------------
        // isAcceptedString
        //
        // Reads through the string passed as parameter to determine whether that string
        // is in the accepted language (returns true) or not (returns false) for the 
        // current CompiledNfaEpsilon
        //-----------------------------------------------------------------------------
        bool isAcceptedString(const string& inputString);

        //-----------------------------------------------------------------------------
        // epsilonClosure
        //
        // Gets each node reachable by epsilon from the currentState and places them
        // into the sourceSet.
        //-----------------------------------------------------------------------------
        static void epsilonClosure(unordered_set<int> &sourceSet, 
            const int currentState, unordered_multimap<string, int> &map);
};

#endif