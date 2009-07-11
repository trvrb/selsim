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
Sequence::Sequence() {

	// filling seq with random nucleotides
	seq = "";	
	for (int i=0; i<LENGTH; i++) {
		char nt = ALPHA[rgen.uniform(0,BASES)]; 
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

/* Mutates ancestor sequence at rate MU per base pair */
/* Proportion ADVPRO multiply fitness by 1+ADVSEL */
/* Proportion DELPRO divide fitness by 1+DELSEL */
void Sequence::newMutant(int mutations) {

	// distributing these mutations uniformly over sequence
	for (int i=0; i<mutations; i++) {
		int loc = rgen.uniform(0,LENGTH); 
		seq.at(loc) = ALPHA[rgen.uniform(0,BASES)];
	}
	
	
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