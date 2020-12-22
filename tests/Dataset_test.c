#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/acutest.h"			
#include "./../include/Dataset.h"



void test_create(void) {

	Dataset * dataset =  createDataset();
	
	TEST_ASSERT(dataset != NULL);
	TEST_ASSERT(dataset->train != NULL);
	TEST_ASSERT(dataset->test != NULL);
	TEST_ASSERT(dataset->validation != NULL);
	
	destroy_Dataset(dataset);
}

void test_insert(void){
	Dataset * dataset =  createDataset();
	DenseMatrix* X = createDenseMatrix();
	X = DenseMatrix_insert(X,1.0,0);

	int y = 0;
	
	insert_toDataset(dataset,X,y,Train);
	TEST_ASSERT(dataset->train->size == 1);
	TEST_ASSERT(dataset->train->y[0] == 0);
	TEST_ASSERT(dataset->train->X[0] != NULL);
	
	destroy_Dataset(dataset);
}


TEST_LIST = {
	{ "create dataset", test_create },
	{ "insert to dataset", test_insert },
	{ NULL, NULL } 
};