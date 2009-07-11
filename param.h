/* param.h
This contains all the global variables used by selsim, can be modified later if necessary
*/

#ifndef PARAM_H
#define PARAM_H

string ALPHA = "ATGC";
int BASES = 4;
double LENGTH = 50;			// length of nucleotide sequence

double MU = 0.2;			// per base per generation mutation rate

double ADVPRO = 0.1;		// proportion of mutations advantageous
double ADVSEL = 0.01;		// selective advantage of advantageous mutants
double DELPRO = 0.5;		// proportion of mutations deleterious
double DELSEL = 0.05;		// selective disadvantage of deleterious mutants

int POPSIZE = 100;			// number of individuals in population

#endif