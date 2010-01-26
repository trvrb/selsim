/* pop.h
Population class definition
This object contains a set of sequence objects.
*/

#ifndef POP_H
#define POP_H

using namespace std;

#include <map>
#include <set>
#include <vector>

class Population {

public:
	Population();									// constructor, creates an initial random sequence at 1.0 count
	
	void evolveStep();								// one generation of evolution, mutation + selection
	
	int sampleAllele();								// outputs a random allele from the population	
	string sampleSeq();								// outputs a random seq from the population
	
	string getSeq(int);								// outputs sequence corresponding to an allele
	double getFitness(int);							// outputs fitness corresponding to an allele
	
	void print(int step);							// go through population and print its contents
	
	int getAlleleCount();
																		
private:
	int alleles;									// number of alleles in population
	vector<Sequence> pop;							// population vector, contains a list of sequences
	
	void mutate();									// select a random individual and mutate it
	void select();									// multinomial sample weighted by fitness
	void purge();									// remove alleles with count = 0
	
};

#endif