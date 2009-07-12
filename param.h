/* param.h
This contains all the global variables used by selsim, can be modified later if necessary
*/

#ifndef PARAM_H
#define PARAM_H

// never modified
string ALPHA = "ATGC";
int BASES = 4;

// defaults
double LENGTH = 100;					// length of nucleotide sequence
double MU = 0.01;						// per base per generation mutation rate
double ADVPRO = 0.0;					// proportion of mutations advantageous
double ADVSEL = 0.0;					// selective advantage of advantageous mutants
double DELPRO = 0.0;					// proportion of mutations deleterious
double DELSEL = 0.0;					// selective disadvantage of deleterious mutants
int POPSIZE = 100;						// number of individuals in population

class Parameters {

public:
	Parameters();						// constructor, imports parameters from selsim.param if available

	void print();						// prints parameter listing
	
private:
	void importLine(string);			// reads a string and attempts to extract parameters from it
	
	
};

#endif