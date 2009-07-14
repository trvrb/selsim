/* sample.h
Sample class definition
This object contains a set of sequence objects marked with their date of collection.
*/

#ifndef SAMPLE_H
#define SAMPLE_H

using namespace std;

#include <map>
#include <set>
#include <vector>

class Sample {

public:
	Sample();								// constructor
	
	void pushBack(string,int);				// add another sample to the list
	void printXML();						// print BEAST-ready XML file of all samples
																		
private:
	int sampleCount;
	vector<string> sampleNames;				// randomly generated names of "0" + 6 characters A-Z
											// "0" here for compatibility with Migrate
	vector<string> sampleSeqs;				// sequences
	vector<double> sampleDates;				// dual dates, with RUNTIME = 0 and generations scaled so that POPSIZE = 1
											// time runs backwards
	
};

#endif