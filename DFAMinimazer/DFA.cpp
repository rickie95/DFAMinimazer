#include "stdafx.h"
#include "DFA.h"

DFA::DFA(SetOfStates Q, Alphabet E, TransitionFunction delta, State q, SetOfStates A) {
	this->Q = Q;
	this->A = A;
	this->E = E;
	this->q = q;
	this->delta = delta;
}

DFA::~DFA() {
	//delta.~TransitionFunction();
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

SetOfStates DFA::Partiziona(DFA dfa, SetOfStates P) {

	return P;
}

DFA DFA::Unisci(DFA dfa, SetOfStates P) {

	return dfa;
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

				if (counter == 1) { // la prima riga riguarda l'insieme degli stati
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
				else if (counter == 2) { // la seconda riga riguarda lo stato iniziale
					q = line;
					counter++;
				}
				else if (counter == 3) { // la terza contiene gi stati accettanti
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
				else if (counter == 4) {// la quarta i simboli dell'alfabeto
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
	}
	else {
		cout << "file non trovato" << endl;
	}
	/* END OF PARSING*/
	cout << " DFA caricato." << endl;
}

void DFA::StampaRiepilogo() {
	cout << "Stati :";
	SetOfStates::iterator it;
	for (it = Q.begin(); it != Q.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Stati accettanti :";
	for (it = A.begin(); it != A.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "Stato iniziale: " << q << endl;
	cout << "Simboli dell'alfabeto :";
	Alphabet::iterator iterator;
	for (iterator = E.begin(); iterator != E.end(); iterator++) {
		cout << *iterator << " ";
	}

	cout << endl;
	cout << "Transizioni: " << endl;

	delta.print();
}