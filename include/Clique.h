#pragma once

#include "./dictionary.h"
#include "./LogisticRegression.h"

#define FALSE 0
#define TRUE 1

typedef struct Clique{

	Set set;
	int * negativeCliques;
	int * unique_negativeCliques;
	size_t numOfUnique_negativeCliques; 
	size_t numOfNegativeCliques; 
	BF* bitArray;
	Dataset * dataset;
	// LogisticRegression * LRModel;

} Clique;

Clique* constructClique();
Clique* addToClique(Clique* myClique, CamSpec* newCamera);
Clique** CreateCliques(DisJointSet * djSet,int* numOfsets);
void destroyCliques(Clique** setsList,int numOfsets);

void trainCliques(Clique** cliqueIndex,int numOfCliques,float learning_rate,float threshold,int max_epochs);
float* testCliques(Clique** cliqueIndex,int numOfCliques);
void GridSearchCliques(Clique * clique,int numOfCliques,FILE * file);
void train_test_split_Cliques(Clique ** cliqueIndex,int numOfCliques);
float* validateCliques(Clique** cliqueIndex,int numOfCliques);
int compareCliques(Clique** cliqueIndex,int cliqueA, int cliqueB);