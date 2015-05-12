/* param.cpp
Member function definitions for Parameter class
Parameter values are all global
*/

#include "param.h"

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

// global random number generator
RNG rgen;

// never modified
int GEN = 0;
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
int RUNTIME = 100;						// number of generations to run simulation
int BURNIN = 90;						// number of generations to run before logging data
int SAMPLECOUNT = 10;					// approximate number of samples to log
int PRINTSTEP = 100;					// print population / screen every x steps
bool PRINTPOPULATION = false;			// print full population of sequences
bool PRINTDIVERSITY = false;			// print pairwise diversity

/* Constructor function to initialize private data */
Parameters::Parameters() {
	
	cout << "Checking parameters" << endl;
	
	// check to see file exists
	string paramString;
	ifstream paramFile ("in.param");
	if (paramFile.is_open()) {
		while (! paramFile.eof() ) {
			getline (paramFile,paramString);
			importLine(paramString);
		}
	}
	else { 
		cout << "Unable to find in.param" << endl; 
		cout << "Running with default parameters" << endl;
	}
	
}

/* Reads a string and attempts to extract parameters from it */
void Parameters::importLine(string line) {

	string pstring = "";		// fill with A-Z
	string vstring = "";		// fill with 0-9 or .

	for (string::iterator is = line.begin(); is != line.end(); is++) {
	
		if (*is == '#')
			break; 
		else if (*is >= 'A' && *is <= 'Z')
    		pstring += *is;
		else if ( (*is >= '0' && *is <= '9') || *is == '.')
    		vstring += *is;
		
	}
	
	double v = atof(vstring.c_str());
	
	if (pstring == "LENGTH")
		LENGTH = v;
	else if (pstring == "MU")
		MU = v;
	else if (pstring == "ADVPRO")
		ADVPRO = v;
	else if (pstring == "ADVSEL")
		ADVSEL = v;
	else if (pstring == "DELPRO")
		DELPRO = v;
	else if (pstring == "DELSEL")
		DELSEL = v;
	else if (pstring == "POPSIZE")
		POPSIZE = v;
	else if (pstring == "RUNTIME")
		RUNTIME = v;
	else if (pstring == "BURNIN")
		BURNIN = v;
	else if (pstring == "SAMPLECOUNT")
		SAMPLECOUNT = v;
	else if (pstring == "PRINTSTEP")
		PRINTSTEP = v;	
	else if (pstring == "PRINTPOPULATION")
		PRINTPOPULATION = true;		
	else if (pstring == "PRINTDIVERSITY")
		PRINTDIVERSITY = true;				
	
}

/* prints parameters */
void Parameters::print() {

	cout << "  sequence length: " << LENGTH << endl;
	cout << "  mutation rate: " << MU << endl;
	cout << "  proportion advantageous: " << ADVPRO << endl;
	cout << "  selective advantage: " << ADVSEL << endl;
	cout << "  proportion deleterious: " << DELPRO << endl;
	cout << "  selective disadvantage: " << DELSEL << endl;
	cout << "  population size: " << POPSIZE << endl;
	cout << "  running time: " << RUNTIME << endl;
	cout << "  burn-in: " << BURNIN << endl;
	cout << "  approximate sample count: " << SAMPLECOUNT << endl;
	cout << "  print population every: " << PRINTSTEP << endl;

}