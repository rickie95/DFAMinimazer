/* 
	DFAMinimazer: based on Hogberg and Larsson (Umea University, Sweden) work "DFA minimisation using the Myhill - Nerode theorem"
	
	Riccardo Malavolti, 2017
*/

#include "stdafx.h"
#include "TransitionFunction.h"
#include "DFA.h"
#include "Superset.h"

DFA* StatiIrraggiungibili(DFA* dfa) {
	
	State s;
	SetOfStates Q, F;
	Transition t;
	TransitionFunction delta;
	Alphabet E = dfa->getAlphabet();
	SetOfStates::iterator it;
	Alphabet::iterator al;

	Q.insert(dfa->getInitilalState()); // lo stato iniziale è sicuramente raggiungibile

	for (it = Q.begin(); it != Q.end(); it++) {
		for (al = E.begin(); al != E.end(); al++) {
			s = dfa->getFinalState(*it, *al);
			if (Q.find(s) == Q.end()) { // se lo stato finale non appartiene a q lo aggiungo
				Q.insert(s);
			}
			if(Q.find(*it) != Q.end()){ // se lo stato inziale è presente in Q allora devo aggiungere la transizione alla nuova fdt
				t.start_state = *it;
				t.simbol = *al;
				t.finish_state = s;
				delta.addTransition(t);
			}
		}
	}
	SetOfStates F1 = dfa->getFinalStates();
	for (it = F1.begin(); it != F1.end(); it++) {
		if (Q.find(*it) != Q.end()) { // se un elemento di F è presente in Q allora lo inserisco in F'
			F.insert(*it);
		}
	}

	DFA* n_dfa = new DFA(Q, dfa->getAlphabet(), delta, dfa->getInitilalState(), F); //l'alfabeto e lo stato iniziale non mutano
	return n_dfa;
}

Superset* RaffinaPartizione(DFA *dfa, Superset* S) {
	Superset* New_P = new Superset();

	SetOfStates::iterator it;
	Alphabet::iterator al;
	string Obs = "";
	SetOfStates allStates= dfa->getAllStates();
	Alphabet alfabet = dfa->getAlphabet();
	for (it = allStates.begin(); it != allStates.end(); it++) {
		Obs = "";
		for (al = alfabet.begin(); al != alfabet.end(); al++) {
			Obs += S->getPartitionIndexByFinalState(dfa->getFinalState(*it, *al));  //creo l'etichetta
		}
		Partition* _ptr = New_P->PartitionByName(Obs);
		if (_ptr != NULL) { // la partizione esiste?
			_ptr->part.insert(*it);
		}
		else {
			SetOfStates s;
			s.insert(*it);
			New_P->addPartition(s, Obs);
		}
	}
	
	if (New_P->isEqual(S))
		return New_P;
	
	return RaffinaPartizione(dfa, New_P);
}
DFA* CollassaStati(DFA* dfa, Superset* S) {


	// I find the start state's partition

	State initial_state = S->getRepresentOf(dfa->getInitilalState());

	// Now I build the new Q and the new transition function
	State s, represent;
	SetOfStates Q, F;
	Alphabet E = dfa->getAlphabet();
	TransitionFunction delta;
	SetOfStates::iterator it;
	Alphabet::iterator al;

	for (it = dfa->getAllStates().begin(); it !=dfa->getAllStates().end(); it++) {
		represent = S->getRepresentOf(*it);
		Q.insert(represent);
		for (al = E.begin(); al != E.end(); al++) {
			s = dfa->getFinalState(*it, *al);
			delta.addTransition(represent, S->getRepresentOf(s), *al);
		}
	}

	// time for F
	for (it = dfa->getFinalStates().begin(); it != dfa->getFinalStates().end(); it++) {
		represent = S->getRepresentOf(*it);
		F.insert(represent);
	}

	dfa->~DFA();
	return new DFA(Q, E, delta, initial_state, F);
}
// DA FARE
/*
	-> Funzione COLLAPSE

*/

int main() {

	SetOfStates Q, A;
	State q;
	TransitionFunction delta;
	Alphabet E;
	DFA *my_dfa = new DFA(Q, E, delta, q, A);

	my_dfa->LoadDFA();

	my_dfa->StampaRiepilogo();

	my_dfa = StatiIrraggiungibili(my_dfa);

	
	Superset* Partizionamento = new Superset();			     // Preparo la partizione {Q\A, A}
	Partizionamento->addPartition(my_dfa->getFinalStates()); // e gli stati accettanti
	Partizionamento->addPartition(my_dfa->Q_meno_A());		 // gli stati non accettanti
	
	cout << "Prima del raffinamento" << endl;
	Partizionamento->print();


	Partizionamento = RaffinaPartizione(my_dfa, Partizionamento);
	cout << "Dopo il raffinamento" << endl;
	Partizionamento->print();
	cout << endl;
	CollassaStati(my_dfa, Partizionamento);
	cout << "Dopo il collasso delle classi di equivalenza" << endl;
	my_dfa->StampaRiepilogo();
	

	system("pause");
	return 0;
} 