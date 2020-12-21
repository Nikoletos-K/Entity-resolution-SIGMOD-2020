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


TEST_LIST = {
	{ "create dataset", test_create },
	{ NULL, NULL } 
};