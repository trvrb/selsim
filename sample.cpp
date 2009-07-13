/* sample.cpp
Member function definitions for Sample class
*/

#include "sample.h"

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
Sample::Sample() {
		
}

void Sample::pushBack(string seq, int gen) {

	char alpha [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R',
		'S','T','U','V','W','X','Y','Z'}; 
	string name = "";
	for (int i = 0; i < 6; i++) {
		int r = rgen.uniform(0,26);
		name.push_back(alpha[r]);
	}

	double date = (RUNTIME - gen) / POPSIZE;
	
	sampleNames.push_back(name);
	sampleSeqs.push_back(seq);
	sampleDates.push_back(date);

}

void Sample::printXML() {

	

}
