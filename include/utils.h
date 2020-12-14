#pragma once

#include "./json.h"
#include "./DisJointSet.h"
#include "./bitArray.h"
#include "./LogisticRegression.h"


#define BUFFER 1024
#define SAME_CAMERAS 1
#define DIFFERENT_CAMERAS 0


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

typedef struct CamerasPair{
	
	CamSpec * camera1;
	CamSpec * camera2;
	int trueLabel;

} CamerasPair;


/* Body of entity resolution */
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs,HashTable* stopwords);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position,HashTable* stopwords);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet *djSet,List * diffPairsList);
void printCameraName(void * data,FILE * output);


/* Clique */
List * printPairs(Clique** setsList,int numOfsets );
void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list);
Clique** CreateSets(DisJointSet * djSet,int* numOfsets);
void destroySets(Clique** setsList,int numOfsets);


/* utils */
int stringComparator(const void * str1,const void * str2);

/* CamerasPair */
CamerasPair * createPair(CamSpec * c1, CamSpec * c2);
void deletePair(CamerasPair * pair);
void setLabel(CamerasPair *  pair,int label);
Clique** createNegConnections(List * diffPairsList,Clique ** CliqueIndex);
Clique * insert_uniqueNegConnection(Clique * cl,int arrayPosition);
Clique * insert_NegConnection(Clique * ql,int arrayPosition);
List * createNegativePairs(Clique ** CliqueIndex,int numOfcliques);

/* Pairs */
CamerasPair ** create_PairsDataset(List * sameCameras,List * differentCameras,int * dataset_size);
void printDataset(CamerasPair ** Dataset,int dataset_size);
void train_test_split(Clique ** cliqueIndex,int numOfCliques);


void addWord(char *word, CamSpec* cs, HashTable* stopwords);

/* Vectorization */

void createVectors(CamSpec ** camArray,int num_of_cameras);
void printVector(CamSpec ** camArray,int num_of_cameras);


void trainCliques(Clique** cliqueIndex,int numOfCliques,float learning_rate,float threshold);
float* testCliques(Clique** cliqueIndex,int numOfCliques);
void GridSearchCliques(Clique * clique,int numOfCliques,FILE * file);
