#pragma once

#include "./HashTable.h"
#include "./list.h"


typedef List * Set;
typedef struct jsonInfo{
	char* key;
	char** value;
	int numOfvalues;
}jsonInfo;

typedef struct CamSpec{
	char* name;
	jsonInfo** infoArray; 
	int numOfSpecs;
	int arrayPosition;
	Set set;
}CamSpec;

jsonInfo * initializeJsonInfo(char* key);
void deleteJsonInfo(jsonInfo* js);
CamSpec * createCamSpec(char * name,int arrayPosition);
CamSpec* addJsonInfo(CamSpec* js,char* key);
void printCamSpec(void * data);
void destroyCamSpec(CamSpec * cs);
int stringComparator(const void * str1,const void * str2);
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);
char** addValue(jsonInfo* js, char* value);
CamSpec * addValuetoCS(CamSpec* cs, char* value);