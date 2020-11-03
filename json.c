#include "json.h"
#include <string.h>


void initializeJsonInfo(char* key, char* value,jsonInfo * jsInfo){
	jsInfo->key = malloc(sizeof(char)*(strlen(key)+1));
	jsInfo->value = malloc(sizeof(char)*(strlen(value)+1));
	strcpy(jsInfo->key,key);
	strcpy(jsInfo->value,value);
}

void deleteJsonInfo(jsonInfo* jsInfo){
	free(jsInfo->key);
	free(jsInfo->value);
	free(jsInfo);
}

void createJson(Json* js){
	js->counter = 0;
	js->infoArray[js->counter] = malloc(sizeof(jsonInfo));
}

void addJsonInfo(Json* js,char* key, char* value){
	js->infoArray[js->counter] = realloc(js->infoArray[js->counter],(js->counter+1));
	initializeJsonInfo(key,value,js->infoArray[js->counter]);
	js->counter +=1;

}