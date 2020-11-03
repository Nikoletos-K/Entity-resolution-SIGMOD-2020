#include <stdio.h>
#include <stdlib.h>

typedef struct jsonInfo{
	char* key;
	char* value;	
}jsonInfo;

typedef struct Json{
	//char* name;
	jsonInfo** infoArray; 
	int counter;
}Json;

void initializeJsonInfo(char* key, char* value,jsonInfo * js);

void deleteJsonInfo(jsonInfo* js);

void createJson(Json* js);

void addJsonInfo(Json* js,char* key, char* value);