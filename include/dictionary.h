#pragma once

#include "./HashTable.h"


typedef struct dictNode{
	int num ;
	int index ;
	char * word;
}dictNode;

extern HashTable * Dictionary;
extern size_t DictionarySize;

extern dictNode ** DictionaryNodes;



HashTable * createStopWords(char* file);

int cmpfunc (const void * a, const void * b);
