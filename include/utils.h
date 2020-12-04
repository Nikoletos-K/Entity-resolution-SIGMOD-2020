#pragma once

#include "./json.h"
#include "./DisJointSet.h"
#include "./bitArray.h"

#define BUFFER 1024
#define SAME_CAMERAS 1
#define DIFFERENT_CAMERAS 0

typedef struct Clique{

	Set set;
	int * negativeCliques;
	size_t numOfNegativeCliques; 
	BF* bitArray;

} Clique;

typedef struct DiffCamerasPair{
	
	CamSpec * camera1;
	CamSpec * camera2;

} DiffCamerasPair;

/* Body of entity resolution */
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet *djSet,List * diffPairsList);
void printCameraName(void * data,FILE * output);


/*clique*/
void printPairs(Clique** setsList,int numOfsets );
Clique** CreateSets(DisJointSet * djSet,int* numOfsets);
void destroySets(Clique** setsList,int numOfsets);

/* utils */
int stringComparator(const void * str1,const void * str2);

/* DiffCamerasPair */
DiffCamerasPair * createPair(CamSpec * c1, CamSpec * c2);
void deletePair(DiffCamerasPair * pair);
Clique** createNegConnections(List * diffPairsList,Clique ** CliqueIndex);

Clique * insert_NegConnection(Clique * ql,int arrayPosition);
