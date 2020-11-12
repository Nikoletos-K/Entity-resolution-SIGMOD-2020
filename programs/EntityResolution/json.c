#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../../include/json.h"

/* --------------- jsonInfo ---------------*/

jsonInfo * initializeJsonInfo(char* key){
	
	jsonInfo * jsInfo = malloc(sizeof(jsonInfo));
	jsInfo->key = strdup(key);
	jsInfo->value = malloc(sizeof(char*));
	jsInfo->numOfvalues = 0;

	return jsInfo;
}


char** addValue(jsonInfo* js, char* value){
	js->value = realloc(js->value,(js->numOfvalues+1)*sizeof(char*));
	// js->value[js->numOfvalues] = strdup(value);
	js->value[js->numOfvalues] = malloc(sizeof(char)*strlen(value)+1);
	strcpy(js->value[js->numOfvalues],value);
	(js->numOfvalues)++;
	return js->value;
}

void deleteJsonInfo(jsonInfo* jsInfo){
	free(jsInfo->key);
	for (int i = 0; i < jsInfo->numOfvalues; i++)
		free(jsInfo->value[i]);
	free(jsInfo->value);
	free(jsInfo);
}

/* --------------- CamSpec ---------------*/


CamSpec* createCamSpec(char * name,int arrayPosition){
	CamSpec* cs = malloc(sizeof(CamSpec));
	cs->numOfSpecs = 0;
	cs->infoArray = malloc(sizeof(jsonInfo*));
	cs->name = strdup(name);
	cs->arrayPosition = arrayPosition;
	cs->set = createList();
	insert_toList(cs->set,cs);

	return cs;
}

CamSpec * addValuetoCS(CamSpec* cs, char* value){
	cs->infoArray[cs->numOfSpecs-1]->value = addValue(cs->infoArray[cs->numOfSpecs-1], value);
	return cs;
}


CamSpec* addJsonInfo(CamSpec* js,char* key){
	js->infoArray = realloc(js->infoArray,(js->numOfSpecs+1)*(sizeof(jsonInfo*)));
	js->infoArray[js->numOfSpecs] = initializeJsonInfo(key);
	js->numOfSpecs +=1;
	return  js;
}

void printCamSpec(void * data){
	printf("%s\n",(char*)(((CamSpec*)data)->name));
}

void destroyCamSpec(CamSpec * cs){
	free(cs->name);
	for(int i=0;i<cs->numOfSpecs;i++)
		deleteJsonInfo(cs->infoArray[i]);
	deleteList(cs->set);
	free(cs->infoArray);
	free(cs);
}