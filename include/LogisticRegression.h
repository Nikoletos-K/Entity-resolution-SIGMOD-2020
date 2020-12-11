#pragma once

#define TRUE 1
#define FALSE 0

#include <stdio.h>

typedef struct LogisticRegression LogisticRegression;

typedef struct LogisticRegression {

	float * weights;
	float bias;
	size_t vectorSize;

} LogisticRegression;


LogisticRegression* LR_construct(size_t vectorSize);
void LR_train(LogisticRegression* model,float * x_vector,int y,float learning_rate,float threshold);
int LR_predict(LogisticRegression* model,float * x_vector);
void LR_destroy(LogisticRegression* model);
float CrossEntropy(float prediction,float * x_vector,float y_vector, size_t vectorSize);

int decision_boundary(float probability);
float sigmoid(float x);
float accuracy(int * prediction_labels,int * true_labels,int numOfLabels);