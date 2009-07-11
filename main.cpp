/*	
	Simple program to do population genetic simulations.  Evolve a population of sequences under
	neutral drift or positive or negative selection.
	
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
#define DEBUG // comment line out to turn off debugging

/* random number generator, global scope */
#include "rng.h"
#include "rng.cpp"
RNG rgen;

/* declaring global parameters, these can be modified later */
double MU = 0.1;

#include "seq.h"
#include "seq.cpp"
#include "pop.h"
#include "pop.cpp"

int main() {			
	
	Sequence initialSeq(50);
	Sequence newSeq = initialSeq;
	
	
	return 0;
}


