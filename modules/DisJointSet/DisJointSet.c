#include <stdio.h>
#include <stdlib.h>

#include "./../../include/DisJointSet.h"



DisJointSet * DSJConstruct(int set_size,void ** objectArray){

	DisJointSet * dsjSet = malloc(sizeof(DisJointSet));
	dsjSet->size = set_size;
	dsjSet->rankArray = calloc(dsjSet->size ,sizeof(int));
	dsjSet->objectArray = objectArray;
	dsjSet->parentArray = malloc(dsjSet->size*sizeof(int));
	for(int i=0;i<set_size;i++)
		dsjSet->parentArray[i] = i;


	return dsjSet;
}

void DSJUnion(DisJointSet * dsjSet,int x,int y){

	DSJConnect(dsjSet,DSJFindParent(dsjSet,x),DSJFindParent(dsjSet,y));
}

void DSJConnect(DisJointSet * dsjSet,int x,int y){

	if(DSJgetRank(dsjSet,x) > DSJgetRank(dsjSet,y))
		DSJsetParent(dsjSet,y,x);
	else{
		DSJsetParent(dsjSet,x,y);
		if(DSJgetRank(dsjSet,x) == DSJgetRank(dsjSet,y))
			DSJsetRank(dsjSet,y,DSJgetRank(dsjSet,y)+1);
	}
}

int DSJFindParent(DisJointSet * dsjSet,int x){

	if(x != DSJgetParent(dsjSet,x))
		DSJsetParent(dsjSet,x,DSJFindParent(dsjSet,DSJgetParent(dsjSet,x)));

	return DSJgetParent(dsjSet,x);
}

int DSJgetRank(DisJointSet * dsjSet,int x){
	return dsjSet->rankArray[x];
}

int DSJgetParent(DisJointSet * dsjSet,int x){
	return dsjSet->parentArray[x];
}

void DSJsetRank(DisJointSet * dsjSet,int x,int newrank){
	dsjSet->rankArray[x] = newrank;
}

void DSJsetParent(DisJointSet * dsjSet,int x,int newparent){
	dsjSet->parentArray[x] = newparent;
}