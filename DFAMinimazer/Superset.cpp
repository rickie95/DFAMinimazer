// #include "stdafx.h" necessary for Visual Studio
#include "Superset.h"

Superset::Superset() {
	num_of_parts = 0;
}
Superset::Superset(Superset *s) {
	num_of_parts = s->getNumOfParts();
	set<Partition, PartitionComparator> * p = s->getPartitions();
	for (set<Partition>::iterator it = p->begin(); it != p->end(); it++) {
		Partition p;
		p.label = it->label;
		SetOfStates s;
		p.part = s;
		for (SetOfStates::iterator set = it->part.begin(); set != it->part.end(); set++) {
			s.insert(*set);
		}
	}
}

void Superset::addPartition(SetOfStates P) {
	Partition part;
	num_of_parts++;
	part.label = std::to_string(num_of_parts);
	part.part = P;
	parts.insert(part);
}
void Superset::addPartition(SetOfStates P, string label) {
	Partition part;
	num_of_parts++;
	part.label = label;
	for (SetOfStates::iterator it = P.begin(); it != P.end(); it++) {
	part.part.insert(*it);
	}
	parts.insert(part);
}


int Superset::getNumOfParts() {
	return num_of_parts;
}

set<Partition, PartitionComparator>* Superset::getPartitions()
{
	return &parts;
}

bool Superset::isEqual(Superset * S) {

	//hanno lo stesso numero di partizioni?
	if (this->size() != S->size())
		return false;

	//confonto ogni partizione, se non ne matcha una -> false
	set<Partition>::iterator it_1, it_2;
	for (it_1 = parts.begin(); it_1 != parts.end(); it_1++) {
		bool match = false;
		for (it_2 = S->getPartitions()->begin(); it_2 != S->getPartitions()->end(); it_2++) {
			if (areEquals(&(it_1->part), &(it_2->part))) { // se trovo almeno una partzione uguale posso continuare
				match = true;
				it_2 = S->getPartitions()->end(); // ehm... ottimizzazione!
				it_2--;
			}
		}
		if (!match) // non ho nessun match di S1 su S2? Posso smettere
			return false;
	}
	return true;
}

void Superset::print() {
	set<Partition>::iterator it;
	int i = num_of_parts;
	for (it = parts.begin(); it != parts.end(); it++) {
		cout << "Partizione #" << num_of_parts - (--i)<<": ";
		for (SetOfStates::iterator set = it->part.begin(); set != it->part.end(); set++) {
			cout << *set << " ";
		}
		cout << endl;
	}
}

unsigned Superset::size() {
	return parts.size();
}

bool Superset::areEquals(const SetOfStates* S1,const SetOfStates* S2) {
	if (S1->size() != S2->size()) // se le partizioni non hanno la stessa dimensione => son diverse
		return false;

	SetOfStates::iterator it_1;

	for (it_1 = S1->begin(); it_1 != S1->end(); it_1++) {
		if (S2->find(*it_1) == S2->end()) // che succede se non trovo un elemento?
			return false; // le due partizioni non sono uguali
	}

	return true; //se hanno la stessa dimensione e gli stessi elementi => sono uguali
}

string Superset::getPartitionIndexByFinalState(State state) {
	for (set<Partition>::iterator it = this->parts.begin(); it != this->parts.end(); it++) {
		if (it->part.find(state) != it->part.end()) {
			return it->label;
		}
	}
	return 0; // La partizione 0 non esiste in nessun superset
}

Partition* Superset::PartitionByName(string label) {
	set<Partition>::iterator it;
	for (it = parts.begin(); it != parts.end(); it++) {
		if (it->label == label)
			return (Partition*)&(*it);
	}

	return NULL;
}

State Superset::getRepresentOf(State s) {
	Partition* p = PartitionByName(getPartitionIndexByFinalState(s));
	SetOfStates::iterator it;
	it = p->part.begin();
	return *it;
}
