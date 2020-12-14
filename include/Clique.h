#pragma once

#include "./dictionary.h"

typedef struct Clique{

	Set set;
	int * negativeCliques;
	int * unique_negativeCliques;
	size_t numOfUnique_negativeCliques; 
	size_t numOfNegativeCliques; 
	BF* bitArray;
	Dataset * dataset;
	LogisticRegression * LRModel;

} Clique;

Clique** CreateCliques(DisJointSet * djSet,int* numOfsets);
void destroyCliques(Clique** setsList,int numOfsets);

void trainCliques(Clique** cliqueIndex,int numOfCliques,float learning_rate,float threshold,int max_epochs);
float* testCliques(Clique** cliqueIndex,int numOfCliques);
void GridSearchCliques(Clique * clique,int numOfCliques,FILE * file);
void train_test_split(Clique ** cliqueIndex,int numOfCliques);
