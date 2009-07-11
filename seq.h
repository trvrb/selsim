/* seq.h
Sequence class definition
This object contains a nucleotide sequence, an associated fitness value, and the sequence's
frequency in the population.
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

	void newMutant();						// produces a new mutant with single individual in the population
		
	string getSeq();						// get functions
	double getFitness();
	double getFreq();
	
	void setFitness(double);				// set functions
	void setFreq(double);
	
private:
	string seq;								// sequence
	double fitness;							// inherited, or set to 1.0 for a new sequence
	double freq;							// set to 1.0 for a new sequence
	
};

#endif