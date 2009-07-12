/* pop.cpp
Member function definitions for Population class
*/

#include "pop.h"

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
Population::Population() {
	Sequence initialSeq;
	pop.push_back(initialSeq);
	alleles = 1;
}

/* Mutate a single individual in the population */
void Population::mutateInd() {

	// select random individual	
	
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
	 
	// mutate this individual
	
	// decrement ancestor allele count
	pop[randAllele].decCount(1);
	
	// create new mutant
	Sequence mutant = pop[randAllele];
	mutant.newMutant();
	pop.push_back(mutant);
	alleles++;

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
	
	// MU*LENGTH*POPSIZE is poisson distributed, 
	// Applying mutations iteratively allows for the proper chance of a double mutant
	int mutations = rgen.poisson(MU*LENGTH*POPSIZE);
	for (int i=0; i<mutations; i++) {
		mutateInd();
	}

	
	#ifdef DEBUG
	cout << "After mutation:" << endl;
	print();
	#endif 

	// SELECTION

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

/* Print pop contents */
void Population::print() {
	cout << alleles << " alleles in the population" << endl;
	for (int i=0; i < alleles; i++) {
		cout << "  Sequence: " << pop[i].getSeq() << endl;
		cout << "    fitness: " << pop[i].getFitness() << endl;
		cout << "    count: " << pop[i].getCount() << endl;
	}
}