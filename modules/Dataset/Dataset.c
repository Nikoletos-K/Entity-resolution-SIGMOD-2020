#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <time.h>

#include "./../../include/Dataset.h"

Dataset * createDataset(){
	
	Dataset * dataset    = malloc(sizeof(Dataset));	
	dataset-> train      = createXy_Split();
	dataset-> test       = createXy_Split();
	dataset-> validation = createXy_Split();
	
	return dataset;
}


Xy_Split * createXy_Split(){

	Xy_Split * Xy = malloc(sizeof(Xy_Split));
	Xy->size = 0;
	Xy->X = malloc(sizeof(DenseMatrix*));
	Xy->y = malloc(sizeof(int));

	return Xy;
}

Dataset * insert_toDataset(Dataset * dataset,DenseMatrix* X,int y,TrainTestVal type){

	switch(type){
		case Train:
			dataset->train->X = realloc(dataset->train->X,(dataset->train->size+1)*sizeof(DenseMatrix*));
			dataset->train->X[dataset->train->size] = X;
			dataset->train->y = realloc(dataset->train->y ,(dataset->train->size+1)*sizeof(int));
			dataset->train->y[dataset->train->size] = y;
			dataset->train->size++;
			break;
		

		case Test:
			dataset->test->X = realloc(dataset->test->X,(dataset->test->size+1)*sizeof(DenseMatrix*));
			dataset->test->X[dataset->test->size] = X;
			dataset->test->y = realloc(dataset->test->y ,(dataset->test->size+1)*sizeof(int));
			dataset->test->y[dataset->test->size] = y;
			dataset->test->size++;
			break;
		

		case Validation:
			dataset->validation->X = realloc(dataset->validation->X,(dataset->validation->size+1)*sizeof(DenseMatrix*));
			dataset->validation->X[dataset->validation->size] = X;
			dataset->validation->y = realloc(dataset->validation->y ,(dataset->validation->size+1)*sizeof(int));
			dataset->validation->y[dataset->validation->size] = y;
			dataset->validation->size++;
			break;
	}

	return dataset;
}

void destroy_XySplit(Xy_Split * Xy){

	for (int i = 0; i < Xy->size; i++)
		if(Xy->X[i]!=NULL)
			destroyDenseMatrix( Xy->X[i] );	
	
	free(Xy->X);
	free(Xy->y);
	free(Xy);
}

void destroy_Dataset(Dataset * dataset){
	
	destroy_XySplit(dataset-> train);
	destroy_XySplit(dataset-> test);
	destroy_XySplit(dataset-> validation);
	free(dataset);
}
