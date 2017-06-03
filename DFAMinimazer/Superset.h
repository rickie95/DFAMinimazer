#pragma once
#include <map>
#include "DFA.h"

using namespace std;

struct Partition {
	string label;
	SetOfStates part;
};

inline unsigned int getha(SetOfStates a) {
	SetOfStates::iterator it;
	unsigned int i = 0;
	for (it = a.begin(); it != a.end(); it++) {
		for (unsigned int j = 0; j < strlen((*it).c_str()); j++) {
			i += (int) ((string)*it)[j];			
		}
	}
	return i;
}

struct PartitionComparator {
	bool operator()(const Partition &a, const Partition &b) const {
		return a.part.size() < b.part.size() || getha(a.part) < getha(b.part);
	}
};

class Superset {
private:
	set<Partition, PartitionComparator> parts;
	int num_of_parts;
public:
	Superset();
	Superset(Superset *s);
	void addPartition(SetOfStates p);
	void addPartition(SetOfStates p, string label);
	void print();
	int getNumOfParts();
	set<Partition, PartitionComparator>* getPartitions();
	bool isEqual(Superset* S);
	unsigned size();
	bool areEquals(const SetOfStates* S1, const SetOfStates* S2);
	string getPartitionIndexByFinalState(State state);
	Partition* PartitionByName(string label);
	State getRepresentOf(State s);
};