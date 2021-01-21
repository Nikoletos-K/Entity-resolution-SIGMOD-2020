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
	
	int max_epochs[5] = {5,10,20,50,100};
	int numOfmax_epochs = 5;


	float threshold_euclid[5] = {1, 0.1 ,0.001,0.0001,0.00001};
	int numofthreshold_euclid = 5;
	float threshold_retrain[2] = {0.01,0.02};
	int numofthreshold_retrain = 2;

	int batch_sizes[2] = {512,1024};
	int numOfbathes = 2;
	int Threads[4] = {5,10,15,20};
	int numThreads = 4;
	
	HyperParameters * hp = constructHyperParameters(learning_rates,numofLr,threshold_euclid,numofthreshold_euclid,threshold_retrain,numofthreshold_retrain,max_epochs,numOfmax_epochs,batch_sizes,numOfbathes,Threads,numThreads);
	
		
	TEST_ASSERT(hp != NULL);
	for (int i = 0; i < numofLr; i++)
		TEST_ASSERT(hp->learning_rates[i] == learning_rates[i]);


	for (int i = 0; i < numOfmax_epochs; i++)
		TEST_ASSERT(hp->max_epochs[i] == max_epochs[i]);
	
	destroyHyperParameters(hp);
	
}

TEST_LIST = {
	{ "create LogisticRegression", test_create },
	{ "create parameters for LR", test_create_hp },
	{ NULL, NULL } 
};