#pragma once

#include <stdio.h>
#include "./utils.h"


typedef struct dictNode{
	int jsonsIn;
	int wordID;
	char * word;
	float averageTfIdf;
	float sumOfTfs;

}dictNode;

extern HashTable * Dictionary;
extern size_t DictionarySize;
extern size_t VectorSize;  
extern dictNode ** DictionaryNodes;



HashTable * createStopWords(char* file);
void addWord(char *word, CamSpec* cs, HashTable* stopwords);
int compareAverageTfIdf (const void * a, const void * b);
dictNode * dictNodeConstruct(int wordID, char* word);
void addTfToDict( CamSpec* cs,dictNode ** DictionaryNodes);
