#pragma once

#include "./list.h"
// #include "./Vectorization.h"
#include "./LogisticRegression.h"

typedef List * Set;

/* struct of json specs */
typedef struct jsonInfo{
	char* key;
	char** value;
	int numOfvalues;
}jsonInfo;

/* Struct that stores every usefull information of a camera */
typedef struct CamSpec{
	char* name;
	jsonInfo** infoArray; 
	int numOfSpecs;
	int arrayPosition;
	Set set;
	int* dictionaryWords;
	int * wordCounters;
	int numOfWords;
	float * vector;
	DenseMatrix * DenseVector;

}CamSpec;

/* jsonInfo functions */
jsonInfo * initializeJsonInfo(char* key);
void deleteJsonInfo(jsonInfo* js);
char** addValue(jsonInfo* js, char* value);
CamSpec * addValuetoCS(CamSpec* cs, char* value);

/* CamSpec functions */
CamSpec * createCamSpec(char * name,int arrayPosition);
CamSpec* addJsonInfo(CamSpec* js,char* key);
void printCamSpec(void * data);
void destroyCamSpec(CamSpec * cs);
