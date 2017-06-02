#pragma once
#include "string"
#include "Set"

using namespace std;
typedef string State;

struct Transition {
	string start_state, finish_state;
	char simbol;
};

struct Comparison{
	bool operator()(const Transition &a, const Transition &b) const{
		return a.start_state < b.start_state || 
			(!(b.start_state < a.start_state) && a.simbol < b.simbol);
	}
};

class TransitionFunction {

private:
	set<Transition, Comparison> delta;

public:
	TransitionFunction();
	TransitionFunction(set<Transition, Comparison> f);
	~TransitionFunction();
	bool addTransition(Transition c);
	void removeTransition(Transition c);
	State getFinalState(string init, char simbol);
	void print();
	
};