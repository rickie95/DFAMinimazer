// #include "stdafx.h" necessary for Visual Studio
#include "TransitionFunction.h"
#include <iostream>

using namespace std;

TransitionFunction::TransitionFunction() { 
	set<Transition, Comparison> f;
	delta = f;
}

TransitionFunction::TransitionFunction(set<Transition, Comparison> f) {
	delta = f;
}

bool TransitionFunction::addTransition(Transition t) {
	return delta.insert(t).second;
}

bool TransitionFunction::addTransition(State start, State end, char symbol) {
	Transition t;
	t.start_state = start;
	t.finish_state = end;
	t.simbol = symbol;
	this->delta.insert(t);
	return true;
}

void TransitionFunction::removeTransition(Transition t) {
	delta.erase(t);
}

TransitionFunction::~TransitionFunction() {
	delta.clear();
}

State TransitionFunction::getFinalState(string init, char simbol) {
	Transition t;
	t.start_state = init;
	t.simbol = simbol;
	set<Transition>::iterator it = delta.find(t);
	if (it == delta.end())
		return NULL;

	return it->finish_state;
}

void TransitionFunction::print() {
	set<Transition>::iterator it;
	for (it = delta.begin(); it != delta.end(); it++)
		cout <<"("<<it->start_state << "," << it->simbol << ") -> " << it->finish_state << endl;
}

set<Transition, Comparison> TransitionFunction::getTransitions()
{
	return this->delta;
}
