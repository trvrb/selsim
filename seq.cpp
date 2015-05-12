/* seq.cpp
Member function definitions for Sequence class
*/

#include "seq.h"
#include "param.h"

/* Constructor function, gives a random sequence of length nucleotides */
Sequence::Sequence() {

	// filling seq with random nucleotides
	seq = "";	
	for (int i=0; i<LENGTH; i++) {
		char nt = ALPHA[rgen.uniform(0,BASES)]; 
		seq.push_back(nt);
	}
	
	// setting fitness of new sequence to 1.0
	fitness = 1.0;
	
	// setting population count to POPSIZE
	count = POPSIZE;
	
}

/* Mutates ancestor sequence */
/* Proportion ADVPRO multiply fitness by 1+ADVSEL */
/* Proportion DELPRO divide fitness by 1+DELSEL */
void Sequence::newMutant() {

	int mutations = 1;

	// distributing these mutations uniformly over sequence
	for (int i=0; i<mutations; i++) {
		int loc = rgen.uniform(0,LENGTH); 
		seq.at(loc) = ALPHA[rgen.uniform(0,BASES)];
	}
	
	// fitness effects multiplicative
	double r = rgen.uniform(0,1);
	if (r < ADVPRO)
		fitness *= 1 + ADVSEL;
	else if (r > ADVPRO && r < ADVPRO + DELPRO)
		fitness /= 1 + DELSEL;
		
	// single individual
	count = 1;
	
}

/* Get functions */
string Sequence::getSeq() { return seq; }
double Sequence::getFitness() { return fitness; }
int Sequence::getCount() { return count; }

/* Set functions */
void Sequence::setFitness(double f) { fitness = f; }
void Sequence::setCount(int c) { count = c; }
void Sequence::decCount(int c) { count -= c; }
