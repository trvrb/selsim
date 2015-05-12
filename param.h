/* param.h
This contains all the global variables used by selsim, can be modified later if necessary
*/

#ifndef PARAM_H
#define PARAM_H

#include "rng.h"

#include <string>
using std::string;

// global random number generator
extern RNG rgen;

extern int GEN;
extern string ALPHA;
extern int BASES;
extern double LENGTH;
extern double MU;
extern double ADVPRO;
extern double ADVSEL;
extern double DELPRO;
extern double DELSEL;
extern int POPSIZE;
extern int RUNTIME;
extern int BURNIN;
extern int SAMPLECOUNT;
extern int PRINTSTEP;
extern bool PRINTPOPULATION;
extern bool PRINTDIVERSITY;

class Parameters {

public:
	Parameters();						// constructor, imports parameters from in.param if available

	void print();						// prints parameter listing
	
private:
	void importLine(string);			// reads a string and attempts to extract parameters from it
	
	
};

#endif