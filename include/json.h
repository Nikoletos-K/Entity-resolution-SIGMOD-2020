#pragma once

typedef struct jsonInfo{
	char* key;
	char* value;	
}jsonInfo;

typedef struct CamSpec{
	char* name;
	jsonInfo** infoArray; 
	int counter;
}CamSpec;

void initializeJsonInfo(char* key, char* value,jsonInfo * js);

void deleteJsonInfo(jsonInfo* js);

CamSpec * createCamSpec(char * name);

CamSpec* addJsonInfo(CamSpec* js,char* key, char* value);