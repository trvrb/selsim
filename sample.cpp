/* sample.cpp
Member function definitions for Sample class
*/

#include "sample.h"
#include "param.h"

#include <fstream>
using std::ofstream;

#include <sstream>
using std::stringstream;

#include <iostream>
using std::cout;
using std::endl;
using std::ios;

/* Constructor function to initialize private data */
Sample::Sample() {
	sampleCount = 0;
	minLabel = 100000;
	maxLabel = 0;
}

void Sample::pushBack(string seq, int gen, double fitness) {

	double date = gen - BURNIN;
	
	sampleCount++;

	sampleSeqs.push_back(seq);
	sampleDates.push_back(date);
	sampleFitnesses.push_back(fitness);
	
}

int Sample::dimen() {
	return maxLabel - minLabel + 1;
}


void Sample::constructNames() {
			
	char alpha [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R',
		'S','T','U','V','W','X','Y','Z'};
	
	for (int i=0; i<sampleCount; i++) {

		/* converting fitness to label */
		/* doesn't work with simultaneous advantagous and deleterious mutation */
		double label = 0;
		if (ADVSEL > 0.000001 || DELSEL > 0.000001) {	
			label = abs(log (sampleFitnesses[i]) / log (1 + ADVSEL + DELSEL));	
		}
		sampleLabels.push_back(label);
		
		if (label < minLabel) { minLabel = label; }
		if (label > maxLabel) { maxLabel = label; }		

		string name = "";
	
		for (int j = 0; j < 6; j++) {
			int r = rgen.uniform(0,26);
			name.push_back(alpha[r]);
		}
		sampleNames.push_back(name);
		
	}

}

void Sample::printFitnesses() {

	if (dimen()>0) {	

		string fitFile( "out.fitness" );
		ofstream fitStream;
		fitStream.open( fitFile.c_str(),ios::out);
	
		for (int i=0; i<sampleCount; i++) {
			fitStream << sampleDates[i] << "\t" << sampleFitnesses[i] << "\t" << sampleLabels[i] << endl;
		}
	
		fitStream.close();
	
	}

}

void Sample::printXML() {

	string xmlFile( "out.beast" );
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
		xmlStream << "\t\t\t<date value=\"" << sampleDates[i] << "\" direction=\"forwards\" units=\"years\"/>" << endl;
		if (dimen()>0) {
			xmlStream << "\t\t\t<attr name=\"fitness\">" <<  sampleLabels[i] << "</attr>" << endl;
		}
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
	
	if (dimen()>0) {	
	
		// FITNESS TYPES
		xmlStream << "\t<generalDataType id=\"fitness\">" << endl;
		for (int i=minLabel; i<=maxLabel; i++) {
			xmlStream << "\t\t<state code=\"" << i << "\"/>" << endl;
		}
		xmlStream << "\t</generalDataType>" << endl;
		xmlStream << endl;	
		
		// FITNESS PATTERNS
		xmlStream << "\t<attributePatterns id=\"fitnessPatterns\" attribute=\"fitness\">" << endl;
		xmlStream << "\t\t<generalDataType idref=\"fitness\"/>" << endl;
		xmlStream << "\t\t<taxa idref=\"taxa\"/>" << endl;
		xmlStream << "\t</attributePatterns>" << endl;
		xmlStream << endl;		
		
	}
	
	// STARTING TREE
	xmlStream << "\t<constantSize id=\"initialDemo\" units=\"years\">" << endl;
	xmlStream << "\t\t<populationSize>" << endl;
	xmlStream << "\t\t\t<parameter id=\"initialDemo.popSize\" value=\"100.0\"/>" << endl;
	xmlStream << "\t\t</populationSize>" << endl;
	xmlStream << "\t</constantSize>" << endl;
	xmlStream << endl;
	xmlStream << "\t<coalescentTree id=\"startingTree\">" << endl;
	xmlStream << "\t\t<taxa idref=\"taxa\"/>" << endl;
	xmlStream << "\t\t<constantSize idref=\"initialDemo\"/>" << endl;
	xmlStream << "\t</coalescentTree>" << endl;
	
	// TREE MODEL
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
	
	// SKYLINE
	xmlStream << "\t<generalizedSkyLineLikelihood id=\"skyline\" linear=\"false\">" << endl;
	xmlStream << "\t\t<populationSizes>" << endl;
	xmlStream << "\t\t\t<parameter id=\"skyline.popSize\" dimension=\"10\" value=\"500.0\" lower=\"0.0\" upper=\"1.0E8\"/>" << endl;
	xmlStream << "\t\t</populationSizes>" << endl;
	xmlStream << "\t\t<groupSizes>" << endl;
	xmlStream << "\t\t\t<parameter id=\"skyline.groupSize\" dimension=\"10\"/>" << endl;
	xmlStream << "\t\t</groupSizes>" << endl;
	xmlStream << "\t\t<populationTree>" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	xmlStream << "\t\t</populationTree>" << endl;
	xmlStream << "\t</generalizedSkyLineLikelihood>" << endl;
	xmlStream << "\t<exponentialMarkovLikelihood id=\"eml1\" jeffreys=\"true\">" << endl;
	xmlStream << "\t\t<chainParameter>" << endl;
	xmlStream << "\t\t\t<parameter idref=\"skyline.popSize\"/>" << endl;
	xmlStream << "\t\t</chainParameter>" << endl;
	xmlStream << "\t</exponentialMarkovLikelihood>" << endl;
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
	xmlStream << "\t\t\t\t<frequencies>" << endl;
	xmlStream << "\t\t\t\t\t<parameter id=\"hky.frequencies\" value=\"0.25 0.25 0.25 0.25\"/>" << endl;
	xmlStream << "\t\t\t\t</frequencies>" << endl;
	xmlStream << "\t\t\t</frequencyModel>" << endl;
	xmlStream << "\t\t</frequencies>" << endl;
	xmlStream << "\t\t<kappa>" << endl;
	xmlStream << "\t\t\t<parameter id=\"hky.kappa\" value=\"1.0\"/>" << endl;
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
	
	if (dimen()>0) {		
	
		// FITNESS SUBSTITUTION MODEL
		xmlStream << "\t<complexSubstitutionModel id=\"fitnessModel\" name=\"origin\">" << endl;
		xmlStream << "\t\t<dataType idref=\"fitness\"/>" << endl;
		xmlStream << "\t\t<rootFrequencies>" << endl;
		xmlStream << "\t\t\t<frequencyModel id=\"fitnessFreqs\" normalize=\"true\">" << endl;
		xmlStream << "\t\t\t\t<dataType idref=\"fitness\"/>" << endl;
		xmlStream << "\t\t\t\t<frequencies>" << endl;
		xmlStream << "\t\t\t\t\t<parameter id=\"fitnessFreqs.frequencies\" dimension=\"" << dimen() << "\"/>" << endl;
		xmlStream << "\t\t\t\t</frequencies>" << endl;
		xmlStream << "\t\t\t</frequencyModel>" << endl;
		xmlStream << "\t\t</rootFrequencies>" << endl;
		xmlStream << "\t\t<rates relativeTo=\"1\">" << endl;
		xmlStream << "\t\t\t<parameter id=\"fitnessRates\" dimension=\"" << dimen() * (dimen()-1) << "\" value=\"1.0\"/>" << endl;		
		xmlStream << "\t\t</rates>" << endl;
		xmlStream << "\t\t<rateIndicator>" << endl;
		xmlStream << "\t\t\t<parameter id=\"fitnessIndicators\" dimension=\"" << dimen() * (dimen()-1) << "\" value=\"1.0\"/>" << endl;
		xmlStream << "\t\t</rateIndicator>" << endl;
		xmlStream << "\t</complexSubstitutionModel>" << endl;
		xmlStream << endl;	
		
		// FITNESS SITE MODEL
		xmlStream << "\t<siteModel id=\"fitnessSiteModel\">" << endl;
		xmlStream << "\t\t<substitutionModel>" << endl;
		xmlStream << "\t\t\t<complexSubstitutionModel idref=\"fitnessModel\"/>" << endl;
		xmlStream << "\t\t</substitutionModel>" << endl;
		xmlStream << "\t\t<mutationRate>" << endl;
		xmlStream << "\t\t\t<parameter id=\"fitnessMu\" value=\"0.005\" lower=\"0.0\" upper=\"10.0\"/>" << endl;
		xmlStream << "\t\t</mutationRate>" << endl;
		xmlStream << "\t</siteModel>" << endl;
		xmlStream << endl;    
		
		// FITNESS TREE MODEL
		xmlStream << "\t<ancestralTreeLikelihood id=\"fitnessTreeLikelihood\">" << endl;
		xmlStream << "\t\t<patterns idref=\"fitnessPatterns\"/>" << endl;
		xmlStream << "\t\t<treeModel idref=\"treeModel\"/>" << endl;
		xmlStream << "\t\t<siteModel idref=\"fitnessSiteModel\"/>" << endl;
		xmlStream << "\t\t<complexSubstitutionModel idref=\"fitnessModel\"/>" << endl;
		xmlStream << "\t</ancestralTreeLikelihood>" << endl;
		xmlStream << endl;     
	
	}

	// OPERATORS //////////////		
	xmlStream << "\t<operators id=\"operators\">" << endl;
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
	xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"15\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"skyline.popSize\"/>" << endl;
	xmlStream << "\t\t</scaleOperator>" << endl;
	xmlStream << "\t\t<deltaExchange delta=\"1\" integer=\"true\" weight=\"6\" autoOptimize=\"false\">" << endl;
	xmlStream << "\t\t\t<parameter idref=\"skyline.groupSize\"/>" << endl;
	xmlStream << "\t\t</deltaExchange>" << endl;
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
	xmlStream << "\t\t</wilsonBalding>" << endl;
	
	if (dimen()>0) {	
	
		xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"10\">" << endl;
		xmlStream << "\t\t\t<parameter idref=\"fitnessMu\"/>" << endl;
		xmlStream << "\t\t</scaleOperator>" << endl;
		xmlStream << "\t\t<scaleOperator scaleFactor=\"0.75\" weight=\"30\" scaleAllIndependently=\"true\" autoOptimize=\"true\">" << endl;
		xmlStream << "\t\t\t<parameter idref=\"fitnessRates\"/>" << endl;
		xmlStream << "\t\t</scaleOperator>" << endl;
	
	}
	
	xmlStream << "\t</operators>" << endl;
	xmlStream << endl;
	
	// MCMC //////////////		
	xmlStream << "\t<mcmc id=\"mcmc\" chainLength=\"1000000000\" autoOptimize=\"true\">" << endl;
	xmlStream << "\t\t<posterior id=\"posterior\">" << endl;
	xmlStream << "\t\t\t<prior id=\"prior\">" << endl;
	xmlStream << "\t\t\t\t<oneOnXPrior>" << endl;
	xmlStream << "\t\t\t\t\t<parameter idref=\"skyline.popSize\"/>" << endl;
	xmlStream << "\t\t\t\t</oneOnXPrior>" << endl;
	xmlStream << "\t\t\t\t<generalizedSkyLineLikelihood idref=\"skyline\"/>" << endl;
	xmlStream << "\t\t\t\t<exponentialMarkovLikelihood idref=\"eml1\"/>" << endl;
		
	if (dimen()>0) {	
		
		xmlStream << endl;
		int d = 0;
		for (int i=maxLabel; i>minLabel; i--) {
			xmlStream << "\t\t\t\t<exponentialPrior mean=\"1.0\" offset=\"0\"> <subStatistic dimension=\"" << d << "\"> <parameter idref=\"fitnessRates\"/> </subStatistic> </exponentialPrior>" << endl;
			d++;
			for (int j=minLabel+1; j<i; j++) {
				xmlStream << "\t\t\t\t<exponentialPrior mean=\"0.001\" offset=\"0\"> <subStatistic dimension=\"" << d << "\"> <parameter idref=\"fitnessRates\"/> </subStatistic> </exponentialPrior>" << endl;
				d++;
			}
		}
		for (int i=maxLabel; i>minLabel; i--) {
			xmlStream << "\t\t\t\t<exponentialPrior mean=\"0.001\" offset=\"0\"> <subStatistic dimension=\"" << d << "\"> <parameter idref=\"fitnessRates\"/> </subStatistic> </exponentialPrior>" << endl;
			d++;
			for (int j=minLabel+1; j<i; j++) {
				xmlStream << "\t\t\t\t<exponentialPrior mean=\"0.001\" offset=\"0\"> <subStatistic dimension=\"" << d << "\"> <parameter idref=\"fitnessRates\"/> </subStatistic> </exponentialPrior>" << endl;
				d++;
			}
		}		
		xmlStream << endl;
		
		xmlStream << "\t\t\t\t<exponentialPrior mean=\"0.005\" offset=\"0\">" << endl;
		xmlStream << "\t\t\t\t\t<parameter idref=\"fitnessMu\"/>" << endl;
		xmlStream << "\t\t\t\t</exponentialPrior>" << endl;
		xmlStream << "\t\t\t\t<complexSubstitutionModel idref=\"fitnessModel\"/>" << endl;	
	
	}
	
	xmlStream << "\t\t\t</prior>" << endl;
	xmlStream << "\t\t\t<likelihood id=\"likelihood\">" << endl;
	xmlStream << "\t\t\t\t<treeLikelihood idref=\"treeLikelihood\"/>" << endl;

	if (dimen()>0) {		
		xmlStream << "\t\t\t\t<ancestralTreeLikelihood idref=\"fitnessTreeLikelihood\"/>" << endl;	
	}
	
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
	if (dimen()>0) {
		xmlStream << "\t\t\t<parameter idref=\"fitnessMu\"/>" << endl;			
	}
	xmlStream << "\t\t\t<parameter idref=\"skyline.popSize\"/>" << endl;
	if (dimen()>0) {	
		xmlStream << "\t\t\t<parameter idref=\"fitnessRates\"/>" << endl;		
	}
	xmlStream << "\t\t</log>" << endl;
	xmlStream << "\t\t<logTree id=\"treeFileLog\" logEvery=\"10000\" nexusFormat=\"true\" fileName=\"selsim.trees\" sortTranslationTable=\"true\">" << endl;
	xmlStream << "\t\t\t<treeModel idref=\"treeModel\"/>" << endl;
	if (dimen()>0) {	
		xmlStream << "\t\t\t<ancestralTreeLikelihood idref=\"fitnessTreeLikelihood\"/>" << endl;	
	}
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