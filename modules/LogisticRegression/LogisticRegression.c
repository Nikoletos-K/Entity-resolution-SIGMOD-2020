#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <time.h>

#include "./../../include/LogisticRegression.h"




LogisticRegression* LR_construct(size_t vectorSize){

	srand(time(NULL));

	LogisticRegression* model = malloc(sizeof(LogisticRegression));
	model->weights    = calloc(vectorSize,sizeof(float));
	model->vectorSize = vectorSize;

	for(int w=0;w<model->vectorSize;w++){
		model->weights[w] = 1/rand();
	}

	model->bias = 1/rand();
	
	return model;
}

void LR_train(LogisticRegression* model,float * x_vector,int y,float learning_rate,float threshold){

	int num_epochs = 100000;
	float gradient;
	float * prev_weights = calloc(model->vectorSize,sizeof(float));
	int converged = TRUE;

	float p_x = LR_predict(model,x_vector);

	while(num_epochs){

		gradient = CrossEntropy(p_x,x_vector,y,model->vectorSize); 
		
		for(int w=0;w<model->vectorSize;w++){
			prev_weights[w]    = model->weights[w];
			model->weights[w] -= learning_rate*gradient;
		}

		model->bias +=  learning_rate*gradient/model->vectorSize;

		for(int w=0;w<model->vectorSize;w++){
			converged = TRUE;
			if(model->weights[w] - prev_weights[w] > threshold){
				converged=FALSE;
				break;
			}
		}

		if(converged)
			break;


		num_epochs--;
	}

}

int LR_predict(LogisticRegression* model,float * x_vector){

	float p_x = 0.0;

	for(int w=0;w<model->vectorSize;w++)
		p_x += model->weights[w]*x_vector[w];

	p_x += model->bias;

	return decision_boundary(sigmoid(p_x));
}

void LR_destroy(LogisticRegression* model){

	free(model->weights);
	free(model);
}

int decision_boundary(float probability){
	return (probability<0.5 ? 0:1);
}

float sigmoid(float x){
	return 1/(1+exp(-x));
}

float CrossEntropy(float prediction,float * x_vector,float y, size_t vectorSize){

	float gradient = 0.0;
	
	for(int i=0;i<vectorSize;i++)
		gradient += (prediction - y)*x_vector[i];
	
	return gradient;
}

float accuracy(int * prediction_labels,int * true_labels,int numOfLabels){

	float acc = 0.0;
	for(int i=0;i<numOfLabels;i++)
		acc += (prediction_labels[i] == true_labels[i] ? 1.0:0.0 );
	
	return (acc/(float)numOfLabels)*100;
}
