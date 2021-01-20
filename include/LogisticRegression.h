#pragma once

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include "./Dataset.h"
#include "./JobScheduler.h"

typedef struct LogisticRegression LogisticRegression;

typedef struct LogisticRegression {

	float * weights;
	float bias;
	size_t vectorSize;
	float learning_rate;
	float threshold;
	float max_epochs;
	int batch_size;
	int numThreads;

} LogisticRegression;

typedef struct threadArgs {

	int batch_first_element;
	int batch_last_element;
	int gradient_position;

} threadArgs;

typedef struct HyperParameters {
	
	float * learning_rates;
	int numofLr;
	float * threshold;
	int numofthreshold;
	int * max_epochs;
	int numOfmax_epochs;
	int batch_size;
	int numThreads;


} HyperParameters;


LogisticRegression* LR_construct(size_t vectorSize,float learning_rate,float threshold,int max_epochs,int batch_size,int numThreads);
void LR_fit(LogisticRegression* model,Xy_Split * Xy_train);
int LR_predict(LogisticRegression* model,DenseMatrix * denseX,int f);
void LR_destroy(LogisticRegression* model);
void LR_Evaluation(LogisticRegression * model,Xy_Split * eval_set,FILE * file);
float CrossEntropy(float prediction,float x,float y, size_t vectorSize);

float LR_predict_proba(LogisticRegression* model,DenseMatrix * denseX);
int decision_boundary(float probability);
float sigmoid(float x);
float accuracy(int * prediction_labels,int * true_labels,int numOfLabels);
float euclid_norm(float * x, int size);

HyperParameters * constructHyperParameters(
	float * learning_rates,
	int numofLr,
	float * threshold,
	int numofthreshold,
	int * max_epochs,
	int numOfmax_epochs,
	int batch_size,
	int numThreads
);

void destroyHyperParameters(HyperParameters * hp);

void GridSearch(Xy_Split * train,Xy_Split * test,HyperParameters * hp,size_t vectorSize,FILE * file);
threadArgs * new_threadArgs(int batch_first_element,int batch_last_element,int gradient_position);