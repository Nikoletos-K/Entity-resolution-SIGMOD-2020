#pragma once

#include "./list.h"
#include "./bitArray.h"
#include "./Vectorization.h"

typedef List * Set;


/* Struct that stores every usefull information of a camera */
typedef struct CamSpec{
	char* name;
	int arrayPosition;
	int myClique;
	Set set;
	BF* bitArray;
	int* dictionaryWords;
	int * wordCounters;
	int numOfWords;
	float * vector;
	DenseMatrix * DenseVector;

}CamSpec;

/* CamSpec functions */
CamSpec * createCamSpec(char * name,int arrayPosition);
CamSpec* addJsonInfo(CamSpec* js,char* key);
void printCamSpec(void * data);
void destroyCamSpec(CamSpec * cs);
