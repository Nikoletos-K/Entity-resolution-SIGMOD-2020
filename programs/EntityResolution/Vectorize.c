#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "./../../include/utils.h"


void createVectors(CamSpec ** camArray,int num_of_cameras){

	qsort(DictionaryNodes, DictionarySize, sizeof(dictNode*), cmpfunc);

	int * dictionaryMap = calloc(DictionarySize,sizeof(int));

	int mapIndex = 1;
	for (int i = DictionarySize-1; i >= DictionarySize-VectorSize; i--){
		int position  = DictionaryNodes[i]->index; 
		dictionaryMap[position] = mapIndex;
		mapIndex++;
			// printf("%d  ----  %s\n",mapIndex, DictionaryNodes[i]->word);
	}

	// float ** bowVectors = malloc(num_of_cameras*sizeof(float*));
	for(int i=0;i<num_of_cameras;i++){

		int numOfWords       = camArray[i]->numOfWords;
		int* dictionaryWords = camArray[i]->dictionaryWords;
		float * bowVectors   = calloc(VectorSize,sizeof(float));
		int length = 0;


		for(int p=0;p<numOfWords;p++){
			
			int vector_position = dictionaryWords[p];
			int final_vector_position = dictionaryMap[vector_position];

			if(final_vector_position!=0){
				length++;
				bowVectors[final_vector_position-1]++;
			}
		}

		
		/*		TF-IDF 		*/
		for(int p=0;p<VectorSize;p++){
			int idf = log(num_of_cameras/DictionaryNodes[p]->num);
			bowVectors[p] /= length;
			bowVectors[p] *= idf;
		}



		camArray[i] -> vector = bowVectors;
	}
}

void printVector(CamSpec ** camArray,int num_of_cameras){

	for(int i=0;i<num_of_cameras;i++){
		printf("%03d:  ",i);
		for(int p=0;p<VectorSize;p+=VectorSize/100){
			printf("%.4f ",camArray[i]->vector[p]);
		}
		printf("\n");
	}	

}
