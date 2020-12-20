#pragma once

#include <stdio.h>

typedef float DMValuetype;

typedef struct DenseMatrixNode {

	DMValuetype value;
	int position;

} DenseMatrixNode;


typedef struct DenseMatrix {

	size_t matrixSize;
	DenseMatrixNode ** matrix;

} DenseMatrix;


DenseMatrixNode * createDenseMatrixNode(DMValuetype value,int position);
DenseMatrix * createDenseMatrix();

DenseMatrix * DenseMatrix_insert(DenseMatrix *  DMatrix,DMValuetype value,int position);


void destroyDenseMatrixNode(DenseMatrixNode * matrixNode);
void destroyDenseMatrix(DenseMatrix * matrix);


DenseMatrix * concatDenseMatrices(DenseMatrix * DMatrix1,DenseMatrix * DMatrix2,size_t vectorSize);
float* concatVectors(float * vector1,float * vector2,int vectorSize);

