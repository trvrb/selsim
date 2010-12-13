/*	
	Simple program to do population genetic simulations.  Evolve a population of sequences under
	neutral drift or positive or negative selection.
	
	Multiplicative fitness.
	Time scaled so that one unit of time is N generations.
	
	Class Sequence holds sequences along with their fitness.
	Class Population holds a set of sequences.  Functions to step population forward in time.
		Needs to store mutation / selection parameters.
	
*/


using namespace std;

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <set>

#define INF pow(double(10),double(100)) // 10^100 (~infinity)
#define PI 3.1415926535897932
//#define DEBUG // comment line out to turn off debugging

/* random number generator, global scope */
#include "rng.h"
#include "rng.cpp"
RNG rgen;

/* declaring global parameters */
#include "param.h"
#include "param.cpp"

/* Sequence class */
#include "seq.h"
#include "seq.cpp"

/* Population class */
#include "pop.h"
#include "pop.cpp"

/* Sample class */
#include "sample.h"
#include "sample.cpp"

int main() {			
	
	Parameters prm;
	Sample s;
	Population p;
	
	prm.print();
	
	int time = 5000;
	int start = RUNTIME-time;	
		
	for (int gen=0; gen<start; gen++) {
	
		// EVOLVE //////////////
		p.evolveStep();
			
		// SAMPLE //////////////	
		if (gen > BURNIN) {
			if (gen % PRINTSTEP == 0) {
				p.print(gen-BURNIN);
			}
			int samples = rgen.poisson( SAMPLECOUNT / (double) (RUNTIME-BURNIN) );
			for (int c=0; c<samples; c++) {
				int allele = p.sampleAllele();
				s.pushBack( p.getSeq(allele), gen, p.getFitness(allele) );
			}
		}
		
		// SCREEN //////////////
		cout << "Step " << gen << "\t";
		for (int j=0; j<p.getAlleleCount(); j += 5) 
			cout << ".";
		cout << endl;
	
	}
	
	// Split off a new population
	Population p2 = p;
	
	// Further evolve population 1
	for (int gen=start; gen<RUNTIME; gen++) {
	
		// EVOLVE //////////////
		p.evolveStep();
			
		int samples = rgen.poisson( ((double) SAMPLECOUNT * 0.5) / (double) (time) );
		for (int c=0; c<samples; c++) {
			int allele = p.sampleAllele();
			s.pushBack( p.getSeq(allele), gen, p.getFitness(allele) );
		}
		
	}	
	
	// Further evolve population 2
	for (int gen=start; gen<RUNTIME; gen++) {
	
		// EVOLVE //////////////
		p2.evolveStep();
			
		int samples = rgen.poisson( ((double) SAMPLECOUNT * 0.5) / (double) (time) );
		for (int c=0; c<samples; c++) {
			int allele = p2.sampleAllele();
			s.pushBack( p2.getSeq(allele), gen, p2.getFitness(allele) );
		}
		
	}		
	
	// SAMPLE OUTPUT //////////////
	s.constructNames();
	s.printXML();
	s.printFitnesses();
		
	return 0;
}
