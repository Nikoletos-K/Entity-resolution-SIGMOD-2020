#pragma once

#include <stdio.h>
#include "./Vectorization.h"

typedef enum TrainTestVal{

	Train,Test,Validation
		
} TrainTestVal;


typedef struct Xy_Split{

	DenseMatrix ** X;
	int * y;
	size_t size;
		
} Xy_Split;


typedef struct Dataset {

	Xy_Split *  train;
	Xy_Split *  validation;
	Xy_Split *  test;
	
} Dataset;

Dataset * createDataset();
Xy_Split * createXy_Split();
Dataset * insert_toDataset(Dataset * dataset,DenseMatrix* X,int y,TrainTestVal type);
void destroy_XySplit(Xy_Split * Xy,int deleteContent);
void destroy_Dataset(Dataset * dataset,int deleteContent);
Xy_Split * insert_toXy_Train(Xy_Split * train,DenseMatrix* X,int y);