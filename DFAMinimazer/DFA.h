/* DFA.h
Contiene le signature dei metodi della classe DFA
L'implementazione è fornita nel file DFA.cpp

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
//typedef set<SetOfStates> Partition;



class DFA {
private:
	SetOfStates Q, A;
	Alphabet E;
	State q;
	TransitionFunction delta;

public:
	DFA(SetOfStates Q, Alphabet E, TransitionFunction delta,  State q, SetOfStates A);
	~DFA();
	SetOfStates getAllStates();
	SetOfStates getFinalStates();
	Alphabet getAlphabet();
	State getInitilalState();
	State getFinalState(string init, char simbol);
	SetOfStates Partiziona(DFA dfa, SetOfStates P);
	DFA Unisci(DFA dfa, SetOfStates P);
	void LoadDFA();
	void StampaRiepilogo();
	SetOfStates Q_meno_A();
};