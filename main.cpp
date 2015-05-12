/*	
	Simple program to do population genetic simulations.  Evolve a population of sequences under
	neutral drift or positive or negative selection.
	
	Multiplicative fitness.
	Time scaled so that one unit of time is N generations.
	
	Class Sequence holds sequences along with their fitness.
	Class Population holds a set of sequences.  Functions to step population forward in time.
		Needs to store mutation / selection parameters.
		
	### Does not handle both ADV and DEL mutations in output at the moment; one or the other work fine.
	
*/


#include <iostream>
using std::cout;
using std::endl;

//#define DEBUG // comment line out to turn off debugging

/* random number generator, global scope */
#include "rng.h"

/* declaring global parameters */
#include "param.h"

/* Sequence class */
#include "seq.h"

/* Population class */
#include "pop.h"

/* Sample class */
#include "sample.h"

int main() {			
	
	Parameters prm;
	Sample s;
	Population p;
	
	prm.print();
			
	for (GEN=0; GEN<RUNTIME; GEN++) {
	
		// EVOLVE //////////////
		p.evolveStep();
			
		// SAMPLE //////////////	
		if (GEN > BURNIN) {
			if (GEN % PRINTSTEP == 0) {
				if (PRINTPOPULATION) { p.print(); }
				if (PRINTDIVERSITY) { p.printDiversity(); }
			}
			int samples = rgen.poisson( SAMPLECOUNT / (double) (RUNTIME-BURNIN) );
			for (int c=0; c<samples; c++) {
				int allele = p.sampleAllele();
				s.pushBack( p.getSeq(allele), GEN, p.getFitness(allele) );
			}
		}
		
		// SCREEN //////////////
			if (GEN % PRINTSTEP == 0) {
			cout << "Step " << GEN << "\t";
			for (int j=0; j<p.getAlleleCount(); j += 5) 
				cout << ".";
			cout << endl;
		}
	
	}
		
	// SAMPLE OUTPUT //////////////
	s.constructNames();
	s.printXML();
	s.printFitnesses();
		
	return 0;
}
