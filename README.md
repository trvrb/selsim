selsim
version 1.0
8/23/2011

Copyright 2011 Trevor Bedford

Please cite:	Bedford T, Cobey S, Pascual M. 2011. Strength and tempo of selection revealed in 
				viral gene genealogies. BMC Evol Biol 11: 220. 

With selsim, an evolving population of sequences is simulated according to a haploid Wright-Fisher 
model with discrete generations.  This uses a Jukes-Cantor mutation model with a specified mutation 
rate. In each subsequent generation, the population is reconstituted by sampling sequences with 
replacement proportional to their frequency multiplied by their fitness. Mutations can be 
advantageous or deleterious and affect fitness in a multiplicative fashion (additive on a log-scale). 
Sequences are sampled at random time points after a period of burn-in. 

Each time there is a mutation that affects fitness, I increment the "fitness class" of that sequence.
This makes it possible, through BEAST's discrete trait models to analyze where and when on the
phylogeny the advantageous or deleterious mutations fall.  However, this only works for situations
with only neutral and deleterious mutations or situations with only neutral and advantageous 
mutations.  Using both deleterious and advantageous mutations, i.e. ADVPRO and DELPRO both > 0, will
work, but output files will not be as interpretable.

Parameters are all in in.param, no re-compile is needed.

Compile with: make
Run with: ./selsim

Output files:

out.beast		Formatted XML output of sequences and dates of all samples.  Set up to run a simple 
				BEAST analysis, with a skyline demographic model and mutational parameters set at 
				their known values.  Effective population sizes, sequence mutation rate are 
				estimated.

With selection (positive or negative):
			
out.fitness		Tabbed-file of sample date, fitness and fitness class for each sample.

out.beast		Includes a discrete-trait analysis of fitness classes over the phylogeny.  Gives a 
				coloring to the tree.  Set up to only allow transitions from fitness class i to i+1.  
				Fitness class mutation rate is estimated.

With PRINTPOPULATION:

out.pop			Every sequence in the population printed in increments of PRINTSTEP.

With PRINTDIVERSITY:

out.diversity	Print the mean pairwise diversity of population of sequences every PRINTSTEP.