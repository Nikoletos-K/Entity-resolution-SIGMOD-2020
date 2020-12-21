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

#include "./../../include/Vectorization.h"


// void createVectors(CamSpec ** camArray,int num_of_cameras){

// 	qsort(DictionaryNodes, DictionarySize, sizeof(dictNode*), cmpfunc);

// 	int * dictionaryMap = calloc(DictionarySize,sizeof(int));

// 	int mapIndex = 1;
// 	for (int i = DictionarySize-1; i >= DictionarySize-VectorSize; i--){
// 		int position  = DictionaryNodes[i]->index; 
// 		dictionaryMap[position] = mapIndex;
// 		mapIndex++;
// 			// printf("%d  ----  %s\n",mapIndex, DictionaryNodes[i]->word);
// 	}

// 	// float ** bowVectors = malloc(num_of_cameras*sizeof(float*));
// 	for(int i=0;i<num_of_cameras;i++){

// 		int numOfWords       = camArray[i]->numOfWords;
// 		int* dictionaryWords = camArray[i]->dictionaryWords;
// 		float * bowVectors   = calloc(VectorSize,sizeof(float));
// 		int length = 0;


// 		for(int p=0;p<numOfWords;p++){
			
// 			int vector_position = dictionaryWords[p];
// 			int final_vector_position = dictionaryMap[vector_position];

// 			if(final_vector_position!=0){
// 				length++;
// 				bowVectors[final_vector_position-1]++;
// 			}
// 		}

		
// 		/*		TF-IDF 		*/
// 		for(int p=0;p<VectorSize;p++){
// 			int idf = log(num_of_cameras/DictionaryNodes[p]->num);
// 			bowVectors[p] /= length;
// 			bowVectors[p] *= idf;
// 		}


// 		camArray[i] -> vector = bowVectors;
// 	}

// 	free(dictionaryMap);
// }


float* concatVectors(float * vector1,float * vector2,int vectorSize){

	float * newVector = malloc(2*vectorSize*sizeof(float));
	memcpy(newVector,vector1,sizeof(float)*vectorSize);
	memcpy(newVector+vectorSize,vector2,sizeof(float)*vectorSize);

	// free(vector1);
	// free(vector2);


	return newVector;
}

// void printVector(CamSpec ** camArray,int num_of_cameras){

// 	for(int i=0;i<num_of_cameras;i++){
// 		printf("\n%03d:  ",i);
// 		for(int p=0;p<VectorSize;p++){
// 			printf("%.4f ",camArray[i]->vector[p]);
// 		}
// 		printf("\n\n");
// 	}	

// }

DenseMatrixNode * createDenseMatrixNode(DMValuetype value,int position){

	DenseMatrixNode * node = malloc(sizeof(DenseMatrixNode));
	node-> value    = value;
	node-> position = position;

	return node;
}


DenseMatrix * createDenseMatrix(){

	DenseMatrix * newMatrix = malloc(sizeof(DenseMatrix));
	newMatrix->matrix = malloc(sizeof(DenseMatrixNode*));
	newMatrix->matrixSize = 0;

	return newMatrix;
}

void destroyDenseMatrixNode(DenseMatrixNode * matrixNode){
	free(matrixNode);
}

void destroyDenseMatrix(DenseMatrix * matrix){

	for(int i=0;i<matrix->matrixSize;i++)
		destroyDenseMatrixNode(matrix->matrix[i]);

	free(matrix->matrix);
	free(matrix);
}


DenseMatrix * DenseMatrix_insert(DenseMatrix *  DMatrix,DMValuetype value,int position){

	DMatrix->matrix = realloc(DMatrix->matrix,(DMatrix->matrixSize+1)*sizeof(DenseMatrixNode*));
	DMatrix->matrix[DMatrix->matrixSize] = createDenseMatrixNode(value,position);
	DMatrix->matrixSize++;

	return DMatrix;
}

DenseMatrix * concatDenseMatrices(DenseMatrix * DMatrix1,DenseMatrix * DMatrix2,size_t vectorSize){

	// size_t newSize = DMatrix1->matrixSize + DMatrix2->matrixSize;

	// printf("--- %ld %ld\n", DMatrix1->matrixSize,DMatrix2->matrixSize);

	DenseMatrix * DMatrix = createDenseMatrix();

	for(int i=0;i<DMatrix1->matrixSize;i++){
		// printf("--%d %lf %d\n",i,DMatrix1->matrix[i]->value ,DMatrix1->matrix[i]->position );
		DMatrix = DenseMatrix_insert(DMatrix,DMatrix1->matrix[i]->value ,DMatrix1->matrix[i]->position );
	}
	
	for(int i=0;i<DMatrix2->matrixSize;i++){
				// printf("--%d %lf %d\n",i,DMatrix2->matrix[i]->value ,DMatrix2->matrix[i]->position );
		DMatrix = DenseMatrix_insert(DMatrix,DMatrix2->matrix[i]->value ,DMatrix2->matrix[i]->position+vectorSize );
	}

	// for(int i=0;i<DMatrix2->matrixSize;i++){
	// 	printf("-- %d %d\n",i,DMatrix2->matrix[i]->position );fflush(stdout);
	// }
	


	// printf("---\n");
	// memcpy(DMatrix->matrix,DMatrix1->matrix,sizeof(DenseMatrixNode*)*(DMatrix1->matrixSize));
	// memcpy(DMatrix->matrix + DMatrix1->matrixSize,DMatrix2->matrix,sizeof(DenseMatrixNode*)*(DMatrix2->matrixSize));

	// for(int i=DMatrix1->matrixSize;i<newSize;i++){
	// 	printf("----%d %d\n",i,DMatrix->matrix[i]->position );fflush(stdout);

	// 	DMatrix->matrix[i]->position += vectorSize; 
	// }

	// free(vector1);
	// free(vector2);


	return DMatrix;	


}