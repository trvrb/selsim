/* param.cpp
Member function definitions for Parameter class
Parameter values are all global
*/

#include "param.h"

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

/* Constructor function to initialize private data */
Parameters::Parameters() {
	
	cout << "Checking parameters" << endl;
	
	// check to see file exists
	string paramString;
	ifstream paramFile ("selsim.param");
	if (paramFile.is_open()) {
		while (! paramFile.eof() ) {
			getline (paramFile,paramString);
			importLine(paramString);
		}
	}
	else { 
		cout << "Unable to find selsim.param" << endl; 
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

}