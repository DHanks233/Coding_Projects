//-----------------------------------------------------------------------------
//	Filename:		NfaToDfa.h
//	Classes:		NfaToDfa
//	Methods:		initializeDfa()
//                  setNfaLanguage()
//                  insertDfaGoalNodes()
//                  insertNfaTransitions
//                  insertDfaNode()
//                  insertDfaTransitions()
//                  processCurrentNode()
//                  setAllDfaNodes()
//                  NfaToDfa()
//                  getDfa()
//
//	Structs:		nodesToProcess
//
//	Written By:		Daniel Hanks
//-----------------------------------------------------------------------------
#ifndef NFATODFA_H
#define NFATODFA_H
#include "CompiledNfaEpsilon.h"

//-----------------------------------------------------------------------------
//	Struct:			nodesToProcess
//	Description:	This struct contains the data members that are transient,
//                  but needed to convert an NFA-e FiniteStateMachine to a DFA
//                  FiniteStateMachine
//
//	Data Members:	unordered_set<int> currentNode: Set containing all current nodes
//                  unordered_set<int> nextNode: Set containing nodes that are
//                                                     next to be processed
//                  queue<unordered_set<int>> nodeQueue: Queue containing all
//                                            sets of nodes not yet processed
//-----------------------------------------------------------------------------
struct nodesToProcess
{
    unordered_set<int> currentNode;
    unordered_set<int> nextNode;
    queue<unordered_set<int>> nodeQueue;
};

typedef unordered_multimap<string, int>::iterator MMI;

//-----------------------------------------------------------------------------
//	Class:			NfaToDfa
//	Description:	This class takes a FiniteStateMachine representing an NFA-e
//                  as parameter for the constructor, and builds a corresponding
//                  FiniteStateMachine representing a DFA
//
//	Data Members:	        
//                  FiniteStateMachine NFA: FSM representing an NFA-e
//                  FiniteStateMachine DFA: FSM representing corresponding DFA
//                  int currentNodeCount: Current number of nodes in DFA
//	                unordered_set<char> nfaLanguage: The language of the NFA
//
//	                unordered_multimap<string, int> transitionsMap: A map that
//                      has source node and transition character as key, and
//                      destination node as the value
//
//	                unordered_map<unordered_set<int>, int> nodeMap: A map that
//                      has an unordered_set of NFA states as the key, and 
//                      the corresponding DFA state as the value
//-----------------------------------------------------------------------------
class NfaToDfa
{
    private:
        FiniteStateMachine NFA;
        FiniteStateMachine DFA;
        int currentNodeCount;
	    unordered_set<char> nfaLanguage;
	    unordered_multimap<string, int> transitionsMap;
	    unordered_map<unordered_set<int>, int> nodeMap;

        void initializeDfa(nodesToProcess &nodesToProcess);
        void setNfaLanguage();
        void insertDfaGoalNodes(const unordered_set<int> currentNode);
        void insertNfaTransitions(const Transition &currentTransition);
        void insertDfaNode(nodesToProcess &nodesToProcess);
        void insertDfaTransition(nodesToProcess &nodesToProcess, const char currentChar);
        void processCurrentNode(nodesToProcess &nodesToProcess, const char transitionChar);
        void setAllDfaNodes(nodesToProcess &nodesToProcess);

    public:
        //-----------------------------------------------------------------------------
        // Constructor
        //
        // Only takes in a FiniteStateMachine as parameter, and converts that FSM into
        // an FSM representing a DFA, which is then stored in the private member: DFA
        //-----------------------------------------------------------------------------
        NfaToDfa(const FiniteStateMachine & newNFA);

        //-----------------------------------------------------------------------------
        // getDfa
        //
        // Returns the DFA private data member
        //-----------------------------------------------------------------------------
        FiniteStateMachine getDfa();
};

//-----------------------------------------------------------------------------
// getDfaFromNfa
//
// This is a stand-alone wrapper function for the NfaToDfa class that returns
// a FiniteStateMachine representing a DFA, derived from the FiniteStateMachine
// parameter passed into the function, which represents an NFA (or DFA).
//-----------------------------------------------------------------------------
FiniteStateMachine getDfaFromNfa(const FiniteStateMachine& newNFA);

#endif