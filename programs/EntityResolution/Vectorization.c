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
	(DMatrix->matrixSize)++;

	return DMatrix;
}

DenseMatrix * concatDenseMatrices(DenseMatrix * DMatrix1,DenseMatrix * DMatrix2,size_t vectorSize){

	DenseMatrix * DMatrix = createDenseMatrix();

	if(DMatrix1==NULL || DMatrix2==NULL)
		return NULL;

	for(int i=0;i<DMatrix1->matrixSize;i++){
		// printf("--%d %lf %d\n",i,DMatrix1->matrix[i]->value ,DMatrix1->matrix[i]->position );
		DMatrix = DenseMatrix_insert(DMatrix,DMatrix1->matrix[i]->value ,DMatrix1->matrix[i]->position );
	}



	for(int i=0;i<DMatrix2->matrixSize;i++){
				// printf("--%d %lf %d\n",i,DMatrix2->matrix[i]->value ,DMatrix2->matrix[i]->position );
		DMatrix = DenseMatrix_insert(DMatrix,DMatrix2->matrix[i]->value ,DMatrix2->matrix[i]->position+vectorSize );
	}
	
	return DMatrix;	

}