#pragma once

#include <stdio.h>

typedef enum TrainTestVal{

	Train,Test,Validation
		
} TrainTestVal;


typedef struct Xy_Split{

	void ** X;
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
Dataset * insert_toDataset(Dataset * dataset,void * X,int y,TrainTestVal type);
void destroy_XySplit(Xy_Split * Xy);
void destroy_Dataset(Dataset * dataset);