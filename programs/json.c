#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../include/json.h"


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

CamSpec* createCamSpec(char * name){
	CamSpec* cs = malloc(sizeof(CamSpec));
	cs->counter = 0;
	cs->infoArray = malloc(sizeof(jsonInfo*));
	cs->infoArray[cs->counter] = malloc(sizeof(jsonInfo));
	cs->name = strdup(name);
	cs->set = NULL;
	cs->set = createList();
	insert_toList(cs->set,cs);

	return cs;
}

CamSpec* addJsonInfo(CamSpec* js,char* key, char* value){
	js->infoArray = realloc(js->infoArray,(js->counter+1));
	initializeJsonInfo(key,value,js->infoArray[js->counter]);
	js->counter +=1;
	return  js;
}

void printCamSpec(void * data){
	printf("%s\n",(char*)(((CamSpec*)data)->name));
}