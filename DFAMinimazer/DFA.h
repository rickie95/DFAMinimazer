/* DFA.h

Riccardo Malavolti, 2017
*/
#pragma once
#include "Set"
#include "TransitionFunction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include<vector>

using namespace std;

typedef string State;
typedef set<State> SetOfStates;
typedef set<char> Alphabet;

class DFA {
private:
	SetOfStates Q, A;
	Alphabet E;
	State q;
	TransitionFunction delta;

public:
	DFA(SetOfStates Q, Alphabet E, TransitionFunction delta,  State q, SetOfStates A);
	DFA();
	~DFA();
	void copyOf(SetOfStates *Q, Alphabet* E, State* q, TransitionFunction *delta, SetOfStates *A);
	SetOfStates getAllStates();
	SetOfStates getFinalStates();
	Alphabet getAlphabet();
	State getInitilalState();
	State getFinalState(string init, char simbol);
	void LoadDFA();
	void PrintDFA();
	SetOfStates Q_meno_A();
};