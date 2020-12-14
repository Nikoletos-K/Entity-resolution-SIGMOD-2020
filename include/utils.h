#pragma once

#include "./CamSpec.h"
#include "./DisJointSet.h"
#include "./bitArray.h"
#include "./LogisticRegression.h"
#include "./HashTable.h"

#define BUFFER 1024
#define SAME_CAMERAS 1
#define DIFFERENT_CAMERAS 0



/* Body of entity resolution */
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs,HashTable* stopwords);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position,HashTable* stopwords);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet *djSet,List * diffPairsList);
void printCameraName(void * data,FILE * output);


/* utils */
int stringComparator(const void * str1,const void * str2);
