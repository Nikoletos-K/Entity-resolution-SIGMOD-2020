#pragma once

#include <stdio.h>
#include "./HashTable.h"


typedef struct dictNode{
	int num;
	int index;
	char * word;
}dictNode;

extern HashTable * Dictionary;
extern size_t DictionarySize;
extern size_t VectorSize;  
extern dictNode ** DictionaryNodes;



HashTable * createStopWords(char* file);

int cmpfunc (const void * a, const void * b);
