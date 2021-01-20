#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./../../include/CamSpec.h"



/* --------------- CamSpec ---------------*/


CamSpec* createCamSpec(char * name,int arrayPosition){
	CamSpec* cs = malloc(sizeof(CamSpec));
	cs->name = strdup(name);
	cs->arrayPosition = arrayPosition;
	cs->myClique = -1;
	cs->set = createList();
	insert_toList(cs->set,cs);
	cs->DenseVector=NULL;

	cs->bitArray = NULL;
	cs->dictionaryWords = malloc(sizeof(int*));
	cs->wordCounters = malloc(sizeof(int*));
	cs->numOfWords = 0;
	return cs;
}


void printCamSpec(void * data){
	printf("%s\n",(char*)(((CamSpec*)data)->name));
}

void destroyCamSpec(CamSpec * cs){
	free(cs->name);
	deleteList(cs->set);
	if(cs->DenseVector!=NULL)
		destroyDenseMatrix(cs->DenseVector);
	// free(cs->vector);
	free(cs->dictionaryWords);
	free(cs->wordCounters);
	free(cs);
}