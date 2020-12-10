#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../include/utils.h"

HashTable * createStopWords(char* file){

	FILE * fp = fopen(file,"r");

	HashTable * ht = HTConstruct(50);

	while(!feof(fp)){
		char buffer[BUFFER];
		fscanf(fp,"%[^\n]\n",buffer);
		buffer[strlen(buffer)-1] = buffer[strlen(buffer)];

		HTInsert(ht,buffer,(void *) buffer,stringComparator);		
	}
	return ht;
}


int cmpfunc (const void * a, const void * b) {

	dictNode * nodeA = *(dictNode**) a;
	dictNode * nodeB = *(dictNode**) b;

   return (nodeA->num - nodeB->num);
}