/* pop.cpp
Member function definitions for Population class
*/

#include "pop.h"
#include "seq.h"
#include "param.h"

#include <fstream>
using std::ofstream;

#include <iostream>
using std::cout;
using std::endl;
using std::ios;

/* Constructor function to initialize private data */
Population::Population() {
	Sequence initialSeq;
	pop.push_back(initialSeq);
	alleles = 1;

	if (PRINTPOPULATION) {
		string popFile( "out.pop" );
		ofstream popStream;
		popStream.open( popFile.c_str(),ios::out);	
		popStream.close();
	}
	
	if (PRINTDIVERSITY) {
		string divFile( "out.diversity" );
		ofstream divStream;
		divStream.open( divFile.c_str(),ios::out);	
		divStream.close();	
	}
	
}

/* Select a random allele in the population */
int Population::sampleAllele() {

	 // construct vector representing the discrete CDF of seq frequences
	 double totalProb = 0;
	 vector<double> seqProbs(alleles);
	 for (int i = 0; i < alleles; i++ ) {
		totalProb += pop[i].getCount() / (double) POPSIZE;
		seqProbs[i] = totalProb;
	 }
	 
	 // drawing a sample with the seq CDF
	 int randAllele;
	 double rindex = rgen.uniform(0,totalProb);
	 for (int i = 0; i < alleles; i++) {
		if (rindex < seqProbs[i]) {
			randAllele = i;
			break;
		}
	 }
	 
	 return randAllele;

}

/* Mutate a single individual in the population */
void Population::mutate() {

	// MU*LENGTH*POPSIZE is poisson distributed, 
	// Applying mutations iteratively allows for the proper chance of a double mutant
	int mutations = rgen.poisson(MU*LENGTH*POPSIZE);
	for (int m=0; m<mutations; m++) {

		// select random individual	
		int randAllele = sampleAllele();
				 	
		// decrement ancestor allele count
		pop[randAllele].decCount(1);
		
		// create new mutant
		Sequence mutant = pop[randAllele];
		mutant.newMutant();
		pop.push_back(mutant);
		alleles++;
	
	}

}

/* Multinomial sample from the population weighted by fitness */
void Population::select() {

	// Multinomial sample with draw probability = frequency * fitness
	vector<double> seqProbs(alleles);
	for (int i=0; i < alleles; i++ ) {
		seqProbs[i] = ( pop[i].getCount() / (double) POPSIZE ) * pop[i].getFitness();
	}
	 
	vector<uint> sample(alleles);
	rgen.multinom(POPSIZE,seqProbs,sample);
	 
	// Modify counts based on multinomial sample
	for (int i=0; i < alleles; i++) {
		pop[i].setCount(sample[i]);
	}

}


/* Remove alleles with count = 0*/
void Population::purge() {
	
	for (vector<Sequence>::iterator it = pop.begin(); it != pop.end(); it++) {
		if ( (*it).getCount() == 0 ) {
			alleles--;
			pop.erase(it);
			it = pop.begin();
		}
	}

}

/* One generation of evolution, mutation + selection */
void Population::evolveStep() {

	// MUTATION
	mutate();	
	
	#ifdef DEBUG
	cout << "After mutation:" << endl;
	print();
	#endif 

	// SELECTION
	select();

	#ifdef DEBUG
	cout << "After selection:" << endl;
	print();
	#endif 
	 
	// HOUSEKEEPING
	purge();

	#ifdef DEBUG
	cout << "After housekeeping:" << endl;
	print();
	#endif 	 
	
}

/* Sample a random sequence from the population NOT weighted by fitness */
string Population::sampleSeq() {
	string s = pop[sampleAllele()].getSeq();
	return s;
}

/* Output a sequence */
string Population::getSeq(int allele) {
	string s = pop[allele].getSeq();
	return s;
}

/* Output a fitness */
double Population::getFitness(int allele) {
	double d = pop[allele].getFitness();
	return d;
}

/* Print pop contents */
void Population::print() {

	string popFile( "out.pop" );
	ofstream popStream;
	popStream.open( popFile.c_str(),ios::app);	

	for (int i=0; i < alleles; i++) {
		for (int j=0; j < pop[i].getCount(); j++) {
			popStream << GEN-BURNIN << "\t" << pop[i].getSeq() << endl;
		}
	}

	popStream.close();

}

int Population::getAlleleCount() {
	return alleles;
}


/* Nucleotide diversity */
double Population::div(string seqA, string seqB) {

	double diff = 0.0;
	int comp = 0;

	while (seqA.size() > 0) {

		string ntA = seqA.substr(0,1);
		seqA.erase(0,1);
		string ntB = seqB.substr(0,1);
		seqB.erase(0,1);
	
		if (ntA.compare("-") != 0 && ntB.compare("-") != 0) {	// ignore gaps
			comp++;
			if ( ntA.compare(ntB) != 0 ) {
				diff++;
			}
		}
		
	}

	return diff / comp;

}

double Population::getDiversity() {
	double d = 0;
	int count = 100;
	for (int i=0; i<count; i++) {
		string seqA = sampleSeq();
		string seqB = sampleSeq();
		d += div(seqA,seqB);
	}
	d = d / (double) count;
	return d;
}

/* Print pop contents */
void Population::printDiversity() {

	string divFile( "out.diversity" );
	ofstream divStream;
	divStream.open( divFile.c_str(),ios::app);	

	divStream << GEN-BURNIN << "\t" << getDiversity() << endl;

	divStream.close();

}