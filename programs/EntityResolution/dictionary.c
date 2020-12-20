#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "./../../include/dictionary.h"

HashTable * createStopWords(char* file){

	FILE * fp = fopen(file,"r");

	HashTable * ht = HTConstruct(50);

	while(!feof(fp)){
		char buffer[BUFFER];
		fscanf(fp,"%[^\n]\n",buffer);
		buffer[strlen(buffer)-1] = buffer[strlen(buffer)];

		HTInsert(ht,buffer,(void *) buffer,stringComparator);		
	}

	fclose(fp);

	return ht;
}


int compareAverageTfIdf (const void * a, const void * b) {

	dictNode * nodeA = *(dictNode**) a;
	dictNode * nodeB = *(dictNode**) b;

	float diff = (((float)nodeA->averageTfIdf - (float)nodeB->averageTfIdf));
   	
   	if(diff>0.0) diff=1;
   	else if(diff== 0.0) diff=0;
   	else diff = -1;

   return diff;
}


void addWord(char *word, CamSpec* cs,HashTable* stopwords){

	for (int i = 0, j; word[i] != '\0'; ++i) {

		if(!islower(word[i]))
			word[i] = tolower(word[i]);

	      // enter the loop if the character is not an alphabet
	      // and not the null character
		while (!(word[i] >= 'a' && word[i] <= 'z') && !(word[i] >= 'A' && word[i] <= 'Z') && !(word[i] == '\0')) {

			for (j = i; word[j] != '\0'; ++j) {
				
				if(!islower(word[j]))
					word[i] = tolower(word[j]);

			// if jth element of line is not an alphabet,
			// assign the value of (j+1)th element to the jth element
				word[j] = word[j + 1];
			}
			word[j] = '\0';
		}
	}


	if(strlen(word)>2 && strlen(word)<11){
		if(HTSearch(stopwords,word,stringComparator)!=NULL)
			return;

		dictNode* node;

		node = (dictNode*) HTSearch(Dictionary,word,stringComparator);

		if(node == NULL){

			cs->dictionaryWords = realloc(cs->dictionaryWords,(cs->numOfWords+1)*sizeof(int));
			cs->wordCounters = realloc(cs->wordCounters,(cs->numOfWords+1)*sizeof(int));


			cs->dictionaryWords[cs->numOfWords] = DictionarySize;
			cs->wordCounters[cs->numOfWords] = 1;

			(cs->numOfWords)++;

			node = dictNodeConstruct(DictionarySize, word);
			
			DictionarySize++;
			DictionaryNodes = realloc(DictionaryNodes,DictionarySize*sizeof(dictNode*));
			DictionaryNodes[DictionarySize-1] = node;
			
			HTInsert(Dictionary,word,(void *) node,stringComparator);
		}else{
			
			int exists=0;

			for(int k=0;k<cs->numOfWords;k++){
				if(cs->dictionaryWords[k] == node->wordID){
					(cs->wordCounters[k])++;
					exists=1;
					break;
				}
			}

			if(!exists){

				cs->dictionaryWords = realloc(cs->dictionaryWords,(cs->numOfWords+1)*sizeof(int));
				cs->wordCounters = realloc(cs->wordCounters,(cs->numOfWords+1)*sizeof(int));


				cs->dictionaryWords[cs->numOfWords] = node->wordID;
				cs->wordCounters[cs->numOfWords] = 1;
				
				(cs->numOfWords)++;

				(node->jsonsIn)++;
			}
		}
	}  
}

dictNode * dictNodeConstruct(int wordID, char* word){
	dictNode* node = malloc(sizeof(dictNode));
	node->jsonsIn = 1;
	node->wordID = wordID;
	node->word = strdup(word);
	node->averageTfIdf = 0.0;
	node->sumOfTfs = 0.0;

	return node;
}

void addTfToDict( CamSpec* cs,dictNode ** DictionaryNodes){

	for (int i = 0; i < cs->numOfWords; i++){
		int wordPosition = cs->dictionaryWords[i];
		DictionaryNodes[wordPosition]->sumOfTfs += (float) (( (float) cs->wordCounters[i])/ ((float) cs->numOfWords));
	}
}
