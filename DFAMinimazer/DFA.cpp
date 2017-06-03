// #include "stdafx.h" necessary for Visual Studio
#include "DFA.h"

DFA::DFA(SetOfStates Q, Alphabet E, TransitionFunction delta, State q, SetOfStates A) {
	this->Q = Q;
	this->A = A;
	this->E = E;
	this->q = q;
	this->delta = delta;
}

DFA::DFA() {};

void DFA::copyOf(SetOfStates *Q, Alphabet* E, State* q, TransitionFunction *delta, SetOfStates *A)
{
	SetOfStates::iterator it;
	for (it = Q->begin(); it != Q->end(); it++) { //Q
		this->Q.insert(*it);
	}
	for (it = A->begin(); it != A->end(); it++) {//F
		this->A.insert(*it);
	}
	Alphabet::iterator al;
	for (al = E->begin(); al != E->end(); al++) {//E
		this->E.insert(*al);
	}
	this->q = *q; //qo
	set<Transition>::iterator trans;
	set<Transition, Comparison> del = delta->getTransitions();
	for (trans = del.begin(); trans != del.end(); trans++) { //trans
		Transition t;
		t.start_state = trans->start_state;
		t.finish_state = trans->finish_state;
		t.simbol = trans->simbol;
		this->delta.addTransition(t);
	}
}

DFA::~DFA() {
	delta.~TransitionFunction();
}


SetOfStates DFA::getAllStates() {
	return Q;
}

SetOfStates DFA::getFinalStates() {
	return A;
}

Alphabet DFA::getAlphabet() {
	return E;
}

State DFA::getInitilalState() {
	return q;
}

State DFA::getFinalState(string init, char simbol) {
	return delta.getFinalState(init, simbol);
}


SetOfStates DFA::Q_meno_A() {
	SetOfStates K;
	SetOfStates::iterator it;
	for (it = Q.begin(); it != Q.end(); it++) 
		if (A.find(*it) == A.end())
			K.insert(*it);

	return K;
}

void DFA::LoadDFA() {
	/* PARSING FILE */
	string name_of_file = "my_DFA.txt";
	ifstream file(name_of_file);
	string line;
	string tokens[3];
	string delimiter = ",";
	int counter = 1;
	if (file.is_open()) {
		while (getline(file, line)) {

			if (line[0] != '#') {

				if (counter == 1) { // first row: states
					stringstream ss(line);
					vector<string> vect;

					while (ss.good()) {
						string substr;
						getline(ss, substr, ',');
						vect.push_back(substr);
					}
					for (unsigned i = 0; i < vect.size(); i++) {
						Q.insert(vect.at(i));
					}
					counter++;
				}
				else if (counter == 2) { // second row: initial state
					q = line;
					counter++;
				}
				else if (counter == 3) { // third row: final states
					stringstream ss(line);
					vector<string> vect;

					while (ss.good()) {
						string substr;
						getline(ss, substr, ',');
						vect.push_back(substr);
					}
					for (unsigned i = 0; i < vect.size(); i++) {
						A.insert(vect.at(i));
					}
					counter++;
				}
				else if (counter == 4) {// fourth row: alphabet
					stringstream ss(line);
					vector<string> vect;

					while (ss.good()) {
						string substr;
						getline(ss, substr, ',');
						vect.push_back(substr);
					}
					for (unsigned i = 0; i < vect.size(); i++) {
						E.insert((vect.at(i))[0]);
					}
					counter++;
				}
				else {
					Transition t;
					t.start_state = line.substr(0, line.find(delimiter));
					line.erase(0, line.find(delimiter) + delimiter.length());

					t.simbol = (line.substr(0, line.find(",")))[0];
					line.erase(0, line.find(delimiter) + delimiter.length());

					t.finish_state = line.substr(0, line.find(","));

					delta.addTransition(t);
				}

			}
		}

		file.close();

		cout << "DFA loaded with success!" << endl;
	}
	else {
		cout << "Error: file not found." << endl;
	}
}

void DFA::PrintDFA() {
	cout << "States :";
	SetOfStates::iterator it;
	for (it = Q.begin(); it != Q.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Final states :";
	for (it = A.begin(); it != A.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Initial state: " << q << endl;
	cout << "Alphabet: ";
	Alphabet::iterator iterator;
	for (iterator = E.begin(); iterator != E.end(); iterator++) {
		cout << *iterator << " ";
	}

	cout << endl;
	cout << "Transitions: " << endl;
	delta.print();
}