/* 
	DFAMinimazer: based on Hogberg and Larsson's (Umea University, Sweden) work "DFA minimisation using the Myhill - Nerode theorem"
	
	Riccardo Malavolti, 2017

	UnreachableStates()     Removes all states unreachable from the initial state.
	RefinePartition()		Creates and refine a partition of Q, at the end produces a partition for every 
							class of states.
	Collapse()				Collapses every partition: if one has more than a state is used a represent (the first in the list).
							Transition function is also modified.
*/

// #include "stdafx.h" necessary for Visual Studio
#include "TransitionFunction.h"
#include "DFA.h"
#include "Superset.h"

DFA* UnreachableStates(DFA* dfa) {
	
	State s;
	SetOfStates Q, F;
	Transition t;
	TransitionFunction delta;
	Alphabet E = dfa->getAlphabet();
	SetOfStates::iterator it;
	Alphabet::iterator al;

	Q.insert(dfa->getInitilalState()); // start state is oblivliosy included

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

Superset* RefinePartition(DFA *dfa, Superset* S) {
	Superset* New_P = new Superset();

	SetOfStates::iterator it;
	Alphabet::iterator al;
	string Obs = "";
	SetOfStates allStates= dfa->getAllStates();
	Alphabet alfabet = dfa->getAlphabet();
	for (it = allStates.begin(); it != allStates.end(); it++) {
		Obs = "";
		for (al = alfabet.begin(); al != alfabet.end(); al++) {
			Obs += S->getPartitionIndexByFinalState(dfa->getFinalState(*it, *al));  //create the label
		}
		Partition* _ptr = New_P->PartitionByName(Obs);
		if (_ptr != NULL) {			// partition exits?
			_ptr->part.insert(*it);
		}
		else {
			SetOfStates s;
			s.insert(*it);
			New_P->addPartition(s, Obs);
		}
	}
	
	if (New_P->isEqual(S)) //if the new partition is equal to the old one, it means we have reached a stable partition
		return New_P;
	
	return RefinePartition(dfa, New_P); // if not we continue with the refinement
}
DFA* Collapse(DFA* dfa, Superset* S) {


	// I find the start state's partition

	State initial_state = S->getRepresentOf(dfa->getInitilalState());

	// Now I build the new Q and the new transition function
	State s, represent;
	SetOfStates Q = dfa->getAllStates(), F = dfa->getFinalStates(), New_Q, New_F;
	Alphabet E = dfa->getAlphabet();
	TransitionFunction delta;
	SetOfStates::iterator it;
	Alphabet::iterator al;

	for (it = Q.begin(); it != Q.end(); it++) {
		represent = S->getRepresentOf(*it);
		New_Q.insert(represent);
		for (al = E.begin(); al != E.end(); al++) {
			s = dfa->getFinalState(*it, *al);
			delta.addTransition(represent, S->getRepresentOf(s), *al);
		}
	}

	// time for F
	for (it = F.begin(); it != F.end(); it++) {
		represent = S->getRepresentOf(*it);
		New_F.insert(represent);
	}
	DFA *n_dfa = new DFA();
	n_dfa->copyOf(&New_Q, &E, &initial_state, &delta, &New_F);
	return n_dfa;
}


int main() {

	SetOfStates Q, A;
	State q;
	TransitionFunction delta;
	Alphabet E;
	DFA *my_dfa = new DFA(Q, E, delta, q, A);
	cout << "########## DFA MINIMAZER ########" << endl;
	cout << " - Riccardo Malavolti, 2017 - " << endl << endl;

	my_dfa->LoadDFA();
	cout << endl << "---- Original DFA ----" << endl;
	my_dfa->PrintDFA();

	my_dfa = UnreachableStates(my_dfa);

	cout << endl << "---- Partitions ----" << endl;
	
	Superset* Partition = new Superset();			     // I make the partition {Q\A, A}
	Partition->addPartition(my_dfa->getFinalStates());   // with final and 
	Partition->addPartition(my_dfa->Q_meno_A());		 // not final states
	
	cout << endl;
	cout << "Before refinement" << endl;
	Partition->print();

	Partition = RefinePartition(my_dfa, Partition);

	cout << endl<<"After refinement" << endl;
	Partition->print();
	cout << endl;

	cout << endl << "---- Minimal DFA ----" << endl;

	my_dfa = Collapse(my_dfa, Partition);
	cout << "After the collapse of equivalence classes, there's the minimal DFA:" << endl;
	my_dfa->PrintDFA();
	

	system("pause");
	return 0;
} 