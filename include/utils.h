#pragma once

#include "./json.h"
#include "./BloomFilter.h"
#include "./DisJointSet.h"


#define BUFFER 1024
#define SAME_CAMERAS 1
#define DIFFERENT_CAMERAS 0

typedef struct Qlique{

	Set set;
	int * negativeQliques;
	size_t numOfNegativeQliques; 

} Qlique;

typedef struct DiffCamerasPair{
	
	CamSpec * camera1;
	CamSpec * camera2;

} DiffCamerasPair;

/* Body of entity resolution */
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet * djSet,List * diffPairsList);
void printCameraName(void * data,FILE * output);
void printPairs(Qlique** setsList,int numOfsets );
Qlique** CreateSets(DisJointSet * djSet,int* numOfsets);
void destroySets(Qlique** setsList,int numOfsets);

/* utils */
int stringComparator(const void * str1,const void * str2);

/* Cantor functions */
int CantorEncode(int num1,int num2);
int CantorDecode(int cantor_number,int* num1,int* num2);

/* DiffCamerasPair */
DiffCamerasPair * createPair(CamSpec * c1, CamSpec * c2);
void deletePair(DiffCamerasPair * pair);
Qlique** createNegConnections(List * diffPairsList,Qlique ** qliqueIndex);

Qlique * insert_NegConnection(Qlique * ql,int arrayPosition);