#pragma once

#include "./HashTable.h"
#include "./list.h"


typedef List * Set;
typedef struct jsonInfo{
	char* key;
	char* value;
}jsonInfo;

typedef struct CamSpec{
	char* name;
	jsonInfo** infoArray; 
	int numOfSpecs;
	int arrayPosition;
	Set set;
}CamSpec;

jsonInfo * initializeJsonInfo(char* key, char* value);
void deleteJsonInfo(jsonInfo* js);
CamSpec * createCamSpec(char * name,int arrayPosition);
CamSpec* addJsonInfo(CamSpec* js,char* key, char* value);
void printCamSpec(void * data);
void destroyCamSpec(CamSpec * cs);
int stringComparator(const void * str1,const void * str2);
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);