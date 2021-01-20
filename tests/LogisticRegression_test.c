#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/acutest.h"			
#include "./../include/LogisticRegression.h"


void test_create(void) {

	size_t vectorSize = 10;
	float learning_rate = 0.1;
	float threshold = 0.1;
	int max_epochs = 2;
	int batch_size=256,numThreads=15;
	float weights = 0.0,bias =0.0;
	LogisticRegression* LR = LR_construct(vectorSize,learning_rate,threshold,max_epochs,batch_size,numThreads);

	
	TEST_ASSERT(LR != NULL);
	TEST_ASSERT(LR->vectorSize == vectorSize);
	TEST_ASSERT(LR->learning_rate == learning_rate);
	TEST_ASSERT(LR->threshold == threshold);
	TEST_ASSERT(LR->max_epochs == max_epochs);

	for(int w=0;w<LR->vectorSize;w++){
		TEST_ASSERT(LR->weights[w] == weights);
	}

	TEST_ASSERT(LR->bias == bias);

	LR_destroy(LR);
}

void test_create_hp(void) {

	float learning_rates[4] =  {1,0.1,0.01,0.001};
	int numofLr = 4;
	float thresholds[5] = {1, 0.1 ,0.001,0.0001,0.00001};
	int numofthreshold = 5;
	int max_epochs[5] = {5,10,20,50,100};
	int numOfmax_epochs = 5;
	int batch_size=256,numThreads=15;
	
	HyperParameters * hp = constructHyperParameters(learning_rates,numofLr,thresholds,numofthreshold,max_epochs,numOfmax_epochs,batch_size,numThreads);
	
		
	TEST_ASSERT(hp != NULL);
	for (int i = 0; i < numofLr; i++)
		TEST_ASSERT(hp->learning_rates[i] == learning_rates[i]);

	for (int i = 0; i < numofthreshold; i++)
		TEST_ASSERT(hp->threshold[i] == thresholds[i]);

	for (int i = 0; i < numOfmax_epochs; i++)
		TEST_ASSERT(hp->max_epochs[i] == max_epochs[i]);
	
	destroyHyperParameters(hp);
	
}

TEST_LIST = {
	{ "create LogisticRegression", test_create },
	{ "create parameters for LR", test_create_hp },
	{ NULL, NULL } 
};