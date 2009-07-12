/* seq.h
Sequence class definition
This object contains a nucleotide sequence, an associated fitness value, and the sequence's
count in the population.
*/

#ifndef SEQ_H
#define SEQ_H

using namespace std;

#include <map>
#include <set>
#include <vector>
#include <string>

class Sequence {

public:
	Sequence();								// constructor, generates a new random sequence

	void newMutant();						// produces a new mutant with a single mutation
		
	string getSeq();						// get functions
	double getFitness();
	int getCount();
	
	void setFitness(double);				// set functions
	void setCount(int);
	
	void decCount(int);						// push count down by x
	
private:
	string seq;								// sequence
	double fitness;							// inherited, or set to 1.0 for a new sequence
	int count;								// set to POPSIZE for a new sequence
	
};

#endif