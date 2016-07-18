//-----------------------------------------------------------------------------
//	Filename:		Homework_4_Driver.cpp
//	Functions:		main()
//
//	Written By:		Daniel P. Hanks
//-----------------------------------------------------------------------------
#include "CompiledDfa.h"
#include "CompiledNfaEpsilon.h"
#include "NfaToDfa.h"
using namespace std;

int main()
{
    FiniteStateMachine f;
    for(int i = 1; i <= 7; i++) {
        f.nodes.insert(i);
    }
    f.goalNodes.insert(3);
    f.goalNodes.insert(5);
    f.goalNodes.insert(6);
    f.goalNodes.insert(7);
    f.startNode = 1;
    f.transitions.emplace_front(1, 0, 2);
    f.transitions.emplace_front(2, 'a', 3);
    f.transitions.emplace_front(3, 'b', 3);
    f.transitions.emplace_front(1, 0, 4);
    f.transitions.emplace_front(4, 'b', 4);
    f.transitions.emplace_front(4, 'c', 5);
    f.transitions.emplace_front(1, 0, 6);
    f.transitions.emplace_front(6, 'a', 6);
    f.transitions.emplace_front(6, 0, 7);
    f.transitions.emplace_front(7, 'c', 7);
    CompiledDfa newDFA = getDfaFromNfa(f);
    CompiledNfaEpsilon newNFA(f);

    cout << boolalpha;
    cout << ": " << newNFA.isAcceptedString("") << endl;
    cout << "abbbb: " << newNFA.isAcceptedString("abbbb") << endl;
    cout << "bbbc: " << newNFA.isAcceptedString("bbbc") << endl;
    cout << "aaaccc: " << newNFA.isAcceptedString("aaaccc") << endl;
    cout << "cccc: " << newNFA.isAcceptedString("cccc") << endl;
    cout << "aaabbb: " << newNFA.isAcceptedString("aaabbb") << endl;
    cout << "a: " << newNFA.isAcceptedString("a") << endl;
    cout << "c: " << newNFA.isAcceptedString("c") << endl;
    cout << "b: " << newNFA.isAcceptedString("b") << endl;
    cout << "aaacccb: " << newNFA.isAcceptedString("aaaccb") << endl << endl;

    cout << ": " << newDFA.isAcceptedString("") << endl;
    cout << "abbbb: " << newDFA.isAcceptedString("abbbb") << endl;
    cout << "bbbc: " << newDFA.isAcceptedString("bbbc") << endl;
    cout << "aaaccc: " << newDFA.isAcceptedString("aaaccc") << endl;
    cout << "cccc: " << newDFA.isAcceptedString("cccc") << endl;
    cout << "aaabbb: " << newDFA.isAcceptedString("aaabbb") << endl;
    cout << "a: " << newDFA.isAcceptedString("a") << endl;
    cout << "c: " << newDFA.isAcceptedString("c") << endl;
    cout << "b: " << newDFA.isAcceptedString("b") << endl;
    cout << "aaacccb: " << newDFA.isAcceptedString("aaacccb") << endl;

    return 0;
}