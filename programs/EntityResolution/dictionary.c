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


	if(strlen(word)>1 && strlen(word)<20){
		if(HTSearch(stopwords,word,stringComparator)!=NULL)
			return;


		cs->dictionaryWords = realloc(cs->dictionaryWords,(cs->numOfWords+1)*sizeof(int));

		dictNode* node;

		node = (dictNode*) HTSearch(Dictionary,word,stringComparator);

		if(node == NULL){

			cs->dictionaryWords[cs->numOfWords] = DictionarySize;

			node = malloc(sizeof(dictNode));
			node->num = 1;
			node->index = DictionarySize;
			node->word = strdup(word);
			node->averageTfIdf = 0.0;
			(cs->numOfWords)++;
			DictionarySize++;
			DictionaryNodes = realloc(DictionaryNodes,DictionarySize*sizeof(dictNode*));
			DictionaryNodes[DictionarySize-1] = node;
			
			HTInsert(Dictionary,word,(void *) node,stringComparator);
		}else{
			cs->dictionaryWords[cs->numOfWords] = node->index;

			int exists=0;
			for(int k=0;k<cs->numOfWords;k++){
				if(cs->dictionaryWords[k] == node->index){
					exists=1;
					break;
				}
			}

			if(!exists)
				(node->num)++;

			(cs->numOfWords)++;
		}
	}  
}

