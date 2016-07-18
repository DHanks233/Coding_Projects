//-----------------------------------------------------------------------------
//	Filename:		NfaToDfa.cpp
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
//	Structs used:	nodesToProcess
//
//	Written By:		Daniel Hanks
//-----------------------------------------------------------------------------
#include "NfaToDfa.h"

//-----------------------------------------------------------------------------
// Constructor
//
// Only takes in a FiniteStateMachine as parameter, and converts that FSM into
// an FSM representing a DFA, which stored in the private member: DFA
//-----------------------------------------------------------------------------
NfaToDfa::NfaToDfa(const FiniteStateMachine & newNFA)
{
    nodesToProcess nodesToProcess;

    NFA = newNFA;
    currentNodeCount = 0;
    setNfaLanguage();
    initializeDfa(nodesToProcess);
    setAllDfaNodes(nodesToProcess);
}

//-----------------------------------------------------------------------------
// initializeDfa
//
// Puts the set of startNodes in the NFA-e and their transitions into the DFA, 
// and places the first set of startNodes into the nodeQueue to be processed
//-----------------------------------------------------------------------------
void NfaToDfa::initializeDfa(nodesToProcess &nodesToProcess)
{
    nodesToProcess.currentNode.insert(NFA.startNode);
	CompiledNfaEpsilon::epsilonClosure(nodesToProcess.currentNode, NFA.startNode, 
        transitionsMap);
	nodeMap.insert(make_pair(nodesToProcess.currentNode, ++currentNodeCount));
	DFA.startNode = currentNodeCount;
	DFA.nodes.insert(currentNodeCount);
	insertDfaGoalNodes(nodesToProcess.currentNode);
    nodesToProcess.nodeQueue.push(nodesToProcess.currentNode);
}

//-----------------------------------------------------------------------------
// setNfaLanguage
//
// Reads through all Transitions in the NFA, stores each transition character
// into the nfaLanguage<char> set, and inserts that transition into transitionsMap
//-----------------------------------------------------------------------------
void NfaToDfa::setNfaLanguage()
{
    for(auto currentTransition = NFA.transitions.begin(); 
        currentTransition != NFA.transitions.end(); currentTransition++)
	{
		if(currentTransition->transitionChar != EPSILON) {
			nfaLanguage.insert(currentTransition->transitionChar);
		}
        insertNfaTransitions(*currentTransition);
	}
}

//-----------------------------------------------------------------------------
// insertDfaGoalNodes
//
// Checks to see if the set of NFA nodes contained in currentNode represents
// a goal state. If it does, it inserts that goal state into the DFA.
//-----------------------------------------------------------------------------
void NfaToDfa::insertDfaGoalNodes(const unordered_set<int> currentNode)
{
	for(auto currentGoal = NFA.goalNodes.begin(); currentGoal != NFA.goalNodes.end() 
		&& DFA.goalNodes.count(currentNodeCount) == 0; currentGoal++) {
		if(currentNode.count(*currentGoal) != 0) {
			DFA.goalNodes.insert(currentNodeCount);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
// insertNfaTransitions
//
// Inserts the current Transition into transitionsMap, using the source node
// and transition character as key, and the destination node as value
//-----------------------------------------------------------------------------
void NfaToDfa::insertNfaTransitions(const Transition &currentTransition)
{
    string key = to_string(currentTransition.source) + currentTransition.transitionChar;
	transitionsMap.insert(make_pair(key, currentTransition.destination));
}

//-----------------------------------------------------------------------------
// insertDfaNode
//
// If the node that is next to process has not already been put into the nodeMap,
// and it is not an empty set (no nodes), place it into the map and add it to
// the nodeQueue to be processed
//-----------------------------------------------------------------------------
void NfaToDfa::insertDfaNode(nodesToProcess &nodesToProcess)
{
    if(nodeMap.count(nodesToProcess.nextNode) == 0 
        && !nodesToProcess.nextNode.empty()) {
	    nodeMap.insert(make_pair(nodesToProcess.nextNode, ++currentNodeCount));
	    DFA.nodes.insert(currentNodeCount);
	    nodesToProcess.nodeQueue.push(nodesToProcess.nextNode);
    }
}

//-----------------------------------------------------------------------------
// insertDfaTransition
//
// Inserts the Transition just determined into the DFA private data member's
// transitions list.
//-----------------------------------------------------------------------------
void NfaToDfa::insertDfaTransition(nodesToProcess &nodesToProcess, const char currentChar)
{
    if(!nodesToProcess.nextNode.empty()){
        Transition tempTrans;
		tempTrans.source = nodeMap.find(nodesToProcess.currentNode)->second;
		tempTrans.destination = nodeMap.find(nodesToProcess.nextNode)->second;
		tempTrans.transitionChar = currentChar;
		DFA.transitions.push_front(tempTrans);
		insertDfaGoalNodes(nodesToProcess.nextNode);
		nodesToProcess.nextNode.clear();
    }
}

//-----------------------------------------------------------------------------
// processCurrentNode
//
// This method gets the possible transitions on the current set of nodes, using
// the current transition character, and puts their destination nodes into 
// nextNode to be processed
//-----------------------------------------------------------------------------
void NfaToDfa::processCurrentNode(nodesToProcess &nodesToProcess, 
                                  const char transitionChar)
{
    pair<MMI, MMI> range;
    for(auto currentState = nodesToProcess.currentNode.begin(); 
        currentState != nodesToProcess.currentNode.end(); currentState++) {
		string key = to_string(*currentState) + transitionChar;
		range = transitionsMap.equal_range(key);
		for(MMI ummIt = range.first; ummIt != range.second; ummIt++) {
			nodesToProcess.nextNode.insert(ummIt->second);
			CompiledNfaEpsilon::epsilonClosure(nodesToProcess.nextNode, 
                ummIt->second, transitionsMap);
		}
	}
}

//-----------------------------------------------------------------------------
// setAllDfaNodes
//
// Runs through each transition character in the language on each set of NFA
// nodes, then inserts the corresponding nodes into the DFA private data member
// along with their transitions.
//-----------------------------------------------------------------------------
void NfaToDfa::setAllDfaNodes(nodesToProcess &nodesToProcess)
{
    while(!nodesToProcess.nodeQueue.empty()) {
	    nodesToProcess.currentNode = nodesToProcess.nodeQueue.front();
	    nodesToProcess.nodeQueue.pop();
	    for(auto currentChar = nfaLanguage.begin(); currentChar != nfaLanguage.end();
            currentChar++) {
		    processCurrentNode(nodesToProcess, *currentChar);
            insertDfaNode(nodesToProcess);
            insertDfaTransition(nodesToProcess, *currentChar);
	    }
    }
}

//-----------------------------------------------------------------------------
// getDfa
//
// Returns the DFA private data member
//-----------------------------------------------------------------------------
FiniteStateMachine NfaToDfa::getDfa()
{
    return DFA;
}

//-----------------------------------------------------------------------------
// getDfaFromNfa
//
// This is a stand-alone wrapper function for the NfaToDfa class that returns
// a FiniteStateMachine representing a DFA, derived from the FiniteStateMachine
// representing an NFA (or DFA) parameter passed into the function.
//-----------------------------------------------------------------------------
FiniteStateMachine getDfaFromNfa(const FiniteStateMachine& newNFA)
{
    NfaToDfa newDFA(newNFA);
    return newDFA.getDfa();
}