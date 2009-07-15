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
	sampleCount = 0;
	startLabel = 0;
}

void Sample::pushBack(string seq, int gen, double fitness) {

	double date = RUNTIME - gen;
	
	sampleCount++;

	sampleSeqs.push_back(seq);
	sampleDates.push_back(date);
	sampleFitnesses.push_back(fitness);
	
	/* converting fitness to label */
	/* doesn't work with simultaneous advantagous and deleterious mutation */
	if (sampleCount == 1) {
		startLabel = abs( log (fitness) / log (1 + ADVSEL + DELSEL) );
	}
	int label = abs( log (fitness) / log (1 + ADVSEL + DELSEL) ) - startLabel;																
	sampleLabels.push_back(label);

	char alpha [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R',
		'S','T','U','V','W','X','Y','Z'}; 
	
	string name;
	stringstream temp;
	temp << label;
	name = temp.str();

	for (int i = 0; i < 6; i++) {
		int r = rgen.uniform(0,26);
		name.push_back(alpha[r]);
	}
	sampleNames.push_back(name);

}

void Sample::printFitnesses() {

	string fitFile( "selsim.fitness" );
	ofstream fitStream;
	fitStream.open( fitFile.c_str(),ios::out);

	for (int i=0; i<sampleCount; i++) {
		fitStream << sampleFitnesses[i] << "\t" << sampleLabels[i] << endl;
	}

	fitStream.close();

}

void Sample::printXML() {

	string xmlFile( "selsim.beast" );
	ofstream xmlStream;
	xmlStream.open( xmlFile.c_str(),ios::out);
	
	// HEADER //////////////
	xmlStream << "<?xml version=\"1.0\" standalone=\"yes\"?>" << endl;
	xmlStream << "<!-- Generated by selsim written by Trevor Bedford -->" << endl;
	xmlStream << "<beast>" << endl;
	xmlStream << endl;
	
	// TAXA //////////////
	xmlStream << "\t<!-- The list of taxa analyse (can also include dates/ages). -->" << endl;
	xmlStream << "\t<!-- ntax=" << sampleCount << " -->" << endl;
	xmlStream << "\t<taxa id=\"taxa\">" << endl;
	for (int i=0; i<sampleCount; i++) {
		xmlStream << "\t\t<taxon id=\"" << sampleNames[i] << "\">" << endl;
		xmlStream << "\t\t\t<date value=\"" << sampleDates[i] << "\" direction=\"backwards\" units=\"years\"/>" << endl;
		xmlStream << "\t\t</taxon>" << endl;
	}
	xmlStream << "\t</taxa>" << endl;
	xmlStream << endl;
	
	// ALIGNMENT //////////////	
	xmlStream << "\t<!-- The sequence alignment (each sequence refers to a taxon above). -->" << endl;
	xmlStream << "\t<!-- ntax=" << sampleCount << " nchar=" << sampleSeqs[0].size() << " -->" << endl;
	xmlStream << "\t<alignment id=\"alignment\" dataType=\"nucleotide\">" << endl;
	for (int i=0; i<sampleCount; i++) {
		xmlStream << "\t\t<sequence>" << endl;
		xmlStream << "\t\t\t<taxon idref=\"" << sampleNames[i] << "\"/>" << endl;
		xmlStream << "\t\t\t" << sampleSeqs[i] << endl;
		xmlStream << "\t\t</sequence>" << endl;
	}	
	xmlStream << "\t</alignment>" << endl;
	xmlStream << endl;	
		
	// POSITIONS //////////////	
	xmlStream << "\t<!-- The unique patterns for all positions -->" << endl;
	xmlStream << "\t<patterns id=\"patterns\" from=\"1\">" << endl;
	xmlStream << "\t\t<alignment idref=\"alignment\"/>" << endl; 
	xmlStream << "\t</patterns>" << endl;
	xmlStream << endl;
	
	// POPULATION SIZE //////////////
	xmlStream << "\t<!-- A prior assumption that the population size has remained constant -->" << endl;
	xmlStream << "\t<!-- throughout the time spanned by the genealogy. -->" << endl;
	xmlStream << "\t<constantSize id=\"constant\" units=\"years\">" << endl;
	xmlStream << "\t\t<populationSize>" << endl;
	xmlStream << "\t\t\t<parameter id=\"constant.popSize\" value=\"" << POPSIZE 	// starting with the neutral expectation
		<< "\" lower=\"0.0\" upper=\"Infinity\"/>" << endl;
	xmlStream << "\t\t</populationSize>" << endl;
	xmlStream << "\t</constantSize>" << endl;
	xmlStream << endl;

	// INITIAL TREE //////////////
	xmlStream << "\t<!-- Construct a rough-and-ready UPGMA tree as an starting tree -->" << endl;
	xmlStream << "\t<upgmaTree id=\"startingTree\">" << endl;
	xmlStream << "\t\t<distanceMatrix correction=\"JC\">" << endl;
	xmlStream << "\t\t\t<patterns>" << endl;
	xmlStream << "\t\t\t\t<alignment idref=\"alignment\"/>" << endl;
	xmlStream << "\t\t\t</patterns>" << endl;
	xmlStream << "\t\t</distanceMatrix>" << endl;
	xmlStream << "\t</upgmaTree>" << endl;
	xmlStream << endl;

	xmlStream << "\t<treeModel id=\"treeModel\">" << endl;
	xmlStream << "\t\t<coalescentTree idref=\"startingTree\"/>" << endl;
	xmlStream << "\t\t<rootHeight>" << endl;
	xmlStream << "\t\t\t<parameter id=\"treeModel.rootHeight\"/>" << endl;
	xmlStream << "\t\t</rootHeight>" << endl;
	xmlStream << "\t\t<nodeHeights internalNodes=\"true\">" << endl;
	xmlStream << "\t\t\t<parameter id=\"treeModel.internalNodeHeights\"/>" << endl;
	xmlStream << "\t\t</nodeHeights>" << endl;
	xmlStream << "\t\t<nodeHeights internalNodes=\"true\" rootNode=\"true\">" << endl;
	xmlStream << "\t\t\t<parameter id=\"treeModel.allInternalNodeHeights\"/>" << endl;
	xmlStream << "\t\t</nodeHeights>" << endl;
	xmlStream << "\t</treeModel>" << endl;
	xmlStream << endl;
	
	xmlStream << "\t<coalescentLikelihood id=\"coalescent\">" << endl;
	xmlStream << "\t\t<model>" << endl;
	xmlStream << "\t\t\t<constantSize idref=\"constant\"/>" << endl;
	xmlStream << "\t\t</model>" << endl;
	xmlStream << "\t\t<populationTree>" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t</populationTree>" << endl;
	xmlStream << "\t</coalescentLikelihood>\t" << endl;
	xmlStream << endl;
	
	// CLOCK //////////////
	xmlStream << "\t<!-- The strict clock (Uniform rates across branches) -->" << endl;
	xmlStream << "\t<strictClockBranchRates id=\"branchRates\">" << endl;
	xmlStream << "\t\t<rate>" << endl;
	xmlStream << "\t\t\t<parameter id=\"clock.rate\" value=\"1.0E-5\" lower=\"0.0\" upper=\"1.0\"/>" << endl;
	xmlStream << "\t\t</rate>" << endl;
	xmlStream << "\t</strictClockBranchRates>" << endl;
	xmlStream << endl;

	// SUBSTITUTION MODEL //////////////	
	xmlStream << "\t<!-- The HKY substitution model (Hasegawa, Kishino & Yano, 1985) -->" << endl;
	xmlStream << "\t<hkyModel id=\"hky\">" << endl;
	xmlStream << "\t\t<frequencies>" << endl;
	xmlStream << "\t\t\t<frequencyModel dataType=\"nucleotide\">" << endl;
	xmlStream << "\t\t\t\t<alignment idref=\"alignment\"/>" << endl;
	xmlStream << "\t\t\t\t<frequencies>" << endl;
	xmlStream << "\t\t\t\t\t<parameter id=\"hky.frequencies\" dimension=\"4\"/>" << endl;
	xmlStream << "\t\t\t\t</frequencies>" << endl;
	xmlStream << "\t\t\t</frequencyModel>" << endl;
	xmlStream << "\t\t</frequencies>" << endl;
	xmlStream << "\t\t<kappa>" << endl;
	xmlStream << "\t\t\t<parameter id=\"hky.kappa\" value=\"1.0\" lower=\"1.0E-8\" upper=\"Infinity\"/>" << endl;
	xmlStream << "\t\t</kappa>" << endl;
	xmlStream << "\t</hkyModel>" << endl;
	xmlStream << endl;
	
	// SITE MODEL //////////////	
	xmlStream << "\t<!-- site model -->" << endl;
	xmlStream << "\t<siteModel id=\"siteModel\">" << endl;
	xmlStream << "\t\t<substitutionModel>" << endl;
	xmlStream << "\t\t\t<hkyModel idref=\"hky\"/>" << endl;
	xmlStream << "\t\t</substitutionModel>" << endl;
	xmlStream << "\t</siteModel>" << endl;
	xmlStream << endl;

	// TREE LIKELIHOOD //////////////		
	xmlStream << "\t<treeLikelihood id=\"treeLikelihood\">" << endl;
	xmlStream << "\t\t<patterns idref=\"patterns\"/>" << endl;
	xmlStream << "\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t<siteModel idref=\"siteModel\"/>" << endl;
	xmlStream << "\t\t<strictClockBranchRates idref=\"branchRates\"/>" << endl;
	xmlStream << "\t</treeLikelihood>" << endl;
	xmlStream << endl;

	// OPERATORS //////////////		
	xmlStream << "\t<operators id=\"operators\">" << endl;
	xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"1\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"hky.kappa\"/>" << endl;
	xmlStream << "\t\t</scaleOperator>" << endl;
	xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"3\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"clock.rate\"/>" << endl;
	xmlStream << "\t\t</scaleOperator>" << endl;
	xmlStream << "\t\t<upDownOperator scaleFactor=\"0.75\" weight=\"3\">" << endl;
	xmlStream << "\t\t\t<up>" << endl;
	xmlStream << "\t\t\t\t<parameter idref=\"clock.rate\"/>" << endl;
	xmlStream << "\t\t\t</up>" << endl;
	xmlStream << "\t\t\t<down>" << endl;
	xmlStream << "\t\t\t\t<parameter idref=\"treeModel.allInternalNodeHeights\"/>" << endl;
	xmlStream << "\t\t\t</down>" << endl;
	xmlStream << "\t\t</upDownOperator>" << endl;
	xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"3\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"constant.popSize\"/>" << endl;
	xmlStream << "\t\t</scaleOperator>" << endl;
	xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"3\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"treeModel.rootHeight\"/>" << endl;
	xmlStream << "\t\t</scaleOperator>" << endl;
	xmlStream << "\t\t<uniformOperator weight=\"30\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"treeModel.internalNodeHeights\"/>" << endl;
	xmlStream << "\t\t</uniformOperator>" << endl;
	xmlStream << "\t\t<subtreeSlide size=\"4.979\" gaussian=\"true\" weight=\"15\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t</subtreeSlide>" << endl;
	xmlStream << "\t\t<narrowExchange weight=\"15\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t</narrowExchange>" << endl;
	xmlStream << "\t\t<wideExchange weight=\"3\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t</wideExchange>" << endl;
	xmlStream << "\t\t<wilsonBalding weight=\"3\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t\t<constantSize idref=\"constant\"/>" << endl;
	xmlStream << "\t\t</wilsonBalding>" << endl;
	xmlStream << "\t</operators>" << endl;
	xmlStream << endl;
	
	// MCMC //////////////		
	xmlStream << "\t<mcmc id=\"mcmc\" chainLength=\"10000000\" autoOptimize=\"true\">" << endl;
	xmlStream << "\t\t<posterior id=\"posterior\">" << endl;
	xmlStream << "\t\t\t<prior id=\"prior\">" << endl;
	xmlStream << "\t\t\t\t<jeffreysPrior>" << endl;
	xmlStream << "\t\t\t\t\t<parameter idref=\"hky.kappa\"/>" << endl;
	xmlStream << "\t\t\t\t</jeffreysPrior>" << endl;
	xmlStream << "\t\t\t\t<jeffreysPrior>" << endl;
	xmlStream << "\t\t\t\t\t<parameter idref=\"constant.popSize\"/>" << endl;
	xmlStream << "\t\t\t\t</jeffreysPrior>" << endl;
	xmlStream << "\t\t\t\t<coalescentLikelihood idref=\"coalescent\"/>" << endl;
	xmlStream << "\t\t\t</prior>" << endl;
	xmlStream << "\t\t\t<likelihood id=\"likelihood\">" << endl;
	xmlStream << "\t\t\t\t<treeLikelihood idref=\"treeLikelihood\"/>" << endl;
	xmlStream << "\t\t\t</likelihood>" << endl;
	xmlStream << "\t\t</posterior>" << endl;
	xmlStream << "\t\t<operators idref=\"operators\"/>" << endl;
	xmlStream << "\t\t<log id=\"screenLog\" logEvery=\"10000\">" << endl;
	xmlStream << "\t\t\t<column label=\"Posterior\" dp=\"4\" width=\"12\">" << endl;
	xmlStream << "\t\t\t\t<posterior idref=\"posterior\"/>" << endl;
	xmlStream << "\t\t\t</column>" << endl;
	xmlStream << "\t\t\t<column label=\"Prior\" dp=\"4\" width=\"12\">" << endl;
	xmlStream << "\t\t\t\t<prior idref=\"prior\"/>" << endl;
	xmlStream << "\t\t\t</column>" << endl;
	xmlStream << "\t\t\t<column label=\"Likelihood\" dp=\"4\" width=\"12\">" << endl;
	xmlStream << "\t\t\t\t<likelihood idref=\"likelihood\"/>" << endl;
	xmlStream << "\t\t\t</column>" << endl;
	xmlStream << "\t\t\t<column label=\"Root Height\" sf=\"6\" width=\"12\">" << endl;
	xmlStream << "\t\t\t\t<parameter idref=\"treeModel.rootHeight\"/>" << endl;
	xmlStream << "\t\t\t</column>" << endl;
	xmlStream << "\t\t\t<column label=\"Rate\" sf=\"6\" width=\"12\">" << endl;
	xmlStream << "\t\t\t\t<parameter idref=\"clock.rate\"/>" << endl;
	xmlStream << "\t\t\t</column>" << endl;
	xmlStream << "\t\t</log>" << endl;
	xmlStream << "\t\t<log id=\"fileLog\" logEvery=\"10000\" fileName=\"selsim.log\">" << endl;
	xmlStream << "\t\t\t<posterior idref=\"posterior\"/>" << endl;
	xmlStream << "\t\t\t<prior idref=\"prior\"/>" << endl;
	xmlStream << "\t\t\t<likelihood idref=\"likelihood\"/>" << endl;
	xmlStream << "\t\t\t<parameter idref=\"clock.rate\"/>" << endl;
	xmlStream << "\t\t\t<parameter idref=\"treeModel.rootHeight\"/>" << endl;
	xmlStream << "\t\t\t<parameter idref=\"constant.popSize\"/>" << endl;
	xmlStream << "\t\t\t<parameter idref=\"hky.kappa\"/>" << endl;
	xmlStream << "\t\t\t<treeLikelihood idref=\"treeLikelihood\"/>" << endl;
	xmlStream << "\t\t\t<coalescentLikelihood idref=\"coalescent\"/>" << endl;
	xmlStream << "\t\t</log>" << endl;
	xmlStream << "\t\t<logTree id=\"treeFileLog\" logEvery=\"10000\" nexusFormat=\"true\" fileName=\"selsim.trees\" sortTranslationTable=\"true\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t\t<posterior idref=\"posterior\"/>" << endl;
	xmlStream << "\t\t</logTree>" << endl;
	xmlStream << "\t</mcmc>" << endl;
	xmlStream << endl;
	
	// REPORT //////////////		
	xmlStream << "\t<report>" << endl;
	xmlStream << "\t\t<property name=\"timer\">" << endl;
	xmlStream << "\t\t\t<object idref=\"mcmc\"/>" << endl;
	xmlStream << "\t\t</property>" << endl;
	xmlStream << "\t</report>" << endl;
	xmlStream << endl;
	
	xmlStream << "</beast>" << endl;
	
	xmlStream.close();

}