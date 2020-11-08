#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../../include/json.h"


jsonInfo * initializeJsonInfo(char* key, char* value){
	jsonInfo * jsInfo = malloc(sizeof(jsonInfo));
	jsInfo->key = strdup(key);
	jsInfo->value = strdup(value);

	return jsInfo;
}

void destroyJsonInfo(jsonInfo * jsInfo){
	free(jsInfo->key);
	free(jsInfo->value);
}

void deleteJsonInfo(jsonInfo* jsInfo){
	free(jsInfo->key);
	free(jsInfo->value);
	free(jsInfo);
}

CamSpec* createCamSpec(char * name,int arrayPosition){
	CamSpec* cs = malloc(sizeof(CamSpec));
	cs->numOfSpecs = 0;
	cs->infoArray = malloc(sizeof(jsonInfo*));
	cs->name = strdup(name);
	cs->arrayPosition = arrayPosition;
	cs->set = NULL;
	cs->set = createList();
	insert_toList(cs->set,cs);

	return cs;
}

CamSpec* addJsonInfo(CamSpec* js,char* key, char* value){
	js->infoArray = realloc(js->infoArray,(js->numOfSpecs+1));
	js->infoArray[js->numOfSpecs] = initializeJsonInfo(key,value);
	js->numOfSpecs +=1;
	return  js;
}

void printCamSpec(void * data){
	printf("%s\n",(char*)(((CamSpec*)data)->name));
}

void destroyCamSpec(CamSpec * cs){
	free(cs->name);
	for(int i=0;i<cs->numOfSpecs;i++){
		destroyJsonInfo(cs->infoArray[i]);
		free(cs->infoArray[i]);
	}
	deleteList(cs->set);
	free(cs->infoArray);
	free(cs);
}