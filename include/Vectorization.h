#pragma once

#include "./dictionary.h"

typedef float DMValuetype;

typedef struct DenseMatrixNode {

	DMValuetype value;
	int position;

} DenseMatrixNode;


typedef struct DenseMatrix {

	size_t matrixSize;
	size_t current_matrixSize;
	DenseMatrixNode ** matrix;

} DenseMatrix;


DenseMatrixNode * createDenseMatrixNode(DMValuetype value,int position);
DenseMatrix * createDenseMatrix(size_t matrixSize);

DenseMatrix * DenseMatrix_insert(DenseMatrix *  DMatrix,DMValuetype value,int position);


void destroyDenseMatrixNode(DenseMatrixNode * matrixNode);
void destroyDenseMatrix(DenseMatrix * matrix);


void createVectors(CamSpec ** camArray,int num_of_cameras);
void printVector(CamSpec ** camArray,int num_of_cameras);
float* concatVectors(float * vector1,float * vector2,int vectorSize);

