#pragma once

#include "./HashTable.h"

typedef HashTable * Set;
typedef struct jsonInfo{
	char* key;
	char* value;	
}jsonInfo;

typedef struct CamSpec{
	char* name;
	jsonInfo** infoArray; 
	int counter;
	Set set;

}CamSpec;

void initializeJsonInfo(char* key, char* value,jsonInfo * js);

void deleteJsonInfo(jsonInfo* js);

CamSpec * createCamSpec(char * name);

CamSpec* addJsonInfo(CamSpec* js,char* key, char* value);

void printCamSpec(void * data);
int stringComparator(const void * str1,const void * str2);