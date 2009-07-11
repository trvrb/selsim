/* seq.cpp
Member function definitions for Sequence class
*/

#include "seq.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <sstream>

/* Constructor function, gives a random sequence of length nucleotides */
Sequence::Sequence(int length) {

	// filling seq with random nucleotides
	seq = "";	
	string alpha = "ATGC";
	
	for (int i=0; i<length; i++) {
		char nt = alpha[rgen.uniform(0,4)]; 
		seq.push_back(nt);
	}
	
	// setting fitness of new sequence to 1.0
	fitness = 1.0;
	
	// setting population frequency to 1.0
	freq = 1.0;

#ifdef DEBUG
	cout << "Sequence: " << seq << endl;
	cout << "  " << "fitness: " << fitness << endl;
	cout << "  " << "frequency: " << freq << endl;
#endif 
	
}


/* Constructor function, inherits properties from another Sequence object */ 
/* Mutates ancestor sequence at rate mu per base pair */
Sequence::Sequence(Sequence const& ancestor) {

	// mutating ancestor sequence, poisson draw and then distributing these mutations uniformly over sequence
//	seq = ancestor.getSeq();
	int length = seq.size();
	string alpha = "ATGC";
	int mutations = rgen.poisson(MU*length);
	
	for (int i=0; i<mutations; i++) {
		int loc = rgen.uniform(0,length); 
		seq.at(loc) = alpha[rgen.uniform(0,4)];
	}
	
	// inheriting fitness
//	fitness = ancestor.getFitness();
	
	// setting population frequency to 1.0
//	freq = 1.0;

#ifdef DEBUG
	cout << "Sequence: " << seq << endl;
	cout << "  " << "fitness: " << fitness << endl;
	cout << "  " << "frequency: " << freq << endl;
#endif 
	
}

/* Get functions */
string Sequence::getSeq() { return seq; }
double Sequence::getFitness() { return fitness; }
double Sequence::getFreq() { return freq; }

/* Set functions */
void Sequence::setFitness(double f) { fitness = f; }
void Sequence::setFreq(double f) { freq = f; }