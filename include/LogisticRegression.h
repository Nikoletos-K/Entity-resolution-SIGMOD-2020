#pragma once

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include "./Dataset.h"

typedef struct LogisticRegression LogisticRegression;

typedef struct LogisticRegression {

	float * weights;
	float bias;
	size_t vectorSize;
	float learning_rate;
	float threshold;
	float max_epochs;

} LogisticRegression;

typedef struct HyperParameters {
	
	float * learning_rates;
	int numofLr;
	float * threshold;
	int numofthreshold;
	int * max_epochs;
	int numOfmax_epochs;


} HyperParameters;


LogisticRegression* LR_construct(size_t vectorSize,float learning_rate,float threshold,int max_epochs);
void LR_fit(LogisticRegression* model,Xy_Split * Xy_train);
int LR_predict(LogisticRegression* model,float * x_vector,int f);
void LR_destroy(LogisticRegression* model);
void LR_Evaluation(LogisticRegression * model,Xy_Split * eval_set,FILE * file);
float CrossEntropy(float prediction,float x,float y, size_t vectorSize);

int decision_boundary(float probability);
float sigmoid(float x);
float accuracy(int * prediction_labels,int * true_labels,int numOfLabels);
float norm(float * x, int size);


HyperParameters * constructHyperParameters(
	float * learning_rates,
	int numofLr,
	float * threshold,
	int numofthreshold,
	int * max_epochs,
	int numOfmax_epochs

);

void destroyHyperParameters(HyperParameters * hp);

void GridSearch(Xy_Split * train,Xy_Split * test,HyperParameters * hp,size_t vectorSize,FILE * file);