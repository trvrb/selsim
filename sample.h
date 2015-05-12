/* sample.h
Sample class definition
This object contains a set of sequence objects marked with their date of collection.
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>
using std::vector;

#include <string>
using std::string;

class Sample {

public:
	Sample();								// constructor
	
	void pushBack(string,int,double);		// add another sample to the list: seq, gen, fitness
	void constructNames();					// takes vector of Fitnesses and constrcts vector of labels 
											// and vector of names

	void printFitnesses();					// print table of samples, fitness and fitness classes
	void printXML();						// print BEAST-ready XML file of all samples
																		
private:
	int sampleCount;
	vector<string> sampleNames;				// randomly generated names of "0" + 6 characters A-Z
											// "0" here for compatibility with Migrate
	vector<string> sampleSeqs;				// sequences
	vector<double> sampleDates;				// dual dates, with RUNTIME = 0 and generations scaled so that POPSIZE = 1
											// time runs backwards
	vector<double> sampleFitnesses;
	vector<double> sampleLabels;			// taken from fitnesses, starts at 0, increases from here
	int minLabel;
	int maxLabel;
	int dimen();							// compares minLabel and maxLabel to give dimensions
	
};

#endif