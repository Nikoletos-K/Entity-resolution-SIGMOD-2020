#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <time.h>

#include "./../../include/LogisticRegression.h"

LogisticRegression* LR_construct(size_t vectorSize,float learning_rate,float threshold,int max_epochs){

	srand(time(NULL));

	LogisticRegression* model = malloc(sizeof(LogisticRegression));
	model->weights    = malloc(vectorSize*sizeof(float));
	model->vectorSize = vectorSize;
	model->learning_rate = learning_rate;
	model->threshold = threshold;
	model->max_epochs = max_epochs;

	for(int w=0;w<model->vectorSize;w++){
		// model->weights[w] = (rand()%2 == 0 ? -1:1)*1/rand();
		// model->weights[w] = (float) 1/(float)rand();
		model->weights[w] = 0.0;
	}

	// model->bias = 1/rand();
	model->bias = 0.0;
	// model->bias = 1.0;

	
	return model;
}

void LR_fit(LogisticRegression* model,Xy_Split * Xy_train){

	int epochs=0,weight_position;
	float loss,value;
	float * prev_weights = calloc(model->vectorSize,sizeof(float));
	int converged    = FALSE;
	DenseMatrix ** X_train  = (DenseMatrix **) Xy_train->X;
	int *  y_train          = Xy_train->y;
	int N                   = Xy_train->size; 

	while(!converged && epochs < model->max_epochs){
	
		float prev_norm = euclid_norm(model->weights,model->vectorSize);

		for(int i=0; i<N; i++){

			DenseMatrix * denseX = X_train[i];
			int denseX_size = denseX->matrixSize;

			float gradient,avg_gradient=0.0;

			/*  Comptute Loss  */
			
			if(denseX_size)
				loss =  LR_predict_proba(model,denseX) - y_train[i];

			for(int p=0; p<denseX_size; p++){

				/*  Update weights  */
				weight_position = denseX->matrix[p]->position;
				value           = denseX->matrix[p]->value;


				gradient           = loss*value;
				avg_gradient      += gradient;

				model->weights[weight_position] -= model->learning_rate*gradient;


			}

			if(denseX_size)
				model->bias -= model->learning_rate*( (float)((float) avg_gradient)/ ((float) N)); 

		}

		float new_norm = euclid_norm(model->weights,model->vectorSize);

		// printf("| %lf | ", fabs(new_norm - prev_norm) );
		fflush(stdout);
		if(fabs(new_norm - prev_norm) < model->threshold){
			printf("\nConverged in %d epochs\n",epochs );
			// free(gradient);
			break;
		}

		// free(gradient);
		epochs++;
	}

	free(prev_weights);
	printf("\n\n");
}

float LR_predict_proba(LogisticRegression* model,DenseMatrix * denseX){

	int denseX_size = denseX->matrixSize,weight_position;
	float p_x = 0.0,value;

	for(int p=0;p<denseX_size;p++){

		weight_position = denseX->matrix[p]->position;
		value           = denseX->matrix[p]->value;

		p_x += model->weights[weight_position]*value;
	}

	p_x += model->bias;

	return sigmoid(p_x);
}

float CrossEntropy(float prediction,float x,float y, size_t vectorSize){

	float gradient = 0.0;
	
	for(int i=0;i<vectorSize;i++)
		gradient += (y-prediction)*x;
	
	return gradient;
}

int LR_predict(LogisticRegression* model,DenseMatrix * denseX,int f){

	int denseX_size = denseX->matrixSize,weight_position;
	float p_x = 0.0,value;

	for(int p=0;p<denseX_size;p++){

		weight_position = denseX->matrix[p]->position;
		value           = denseX->matrix[p]->value;

		p_x += model->weights[weight_position]*value;
	}

	p_x += model->bias;

	if(f==1)
		printf("(%.5lf) - ",sigmoid(p_x));
	return decision_boundary(sigmoid(p_x));
}

void LR_destroy(LogisticRegression* model){

	free(model->weights);
	free(model);
}

int decision_boundary(float propability){
	//printf("(%.5lf) - ",propability);
	return (propability<=0.5 ? 0:1);
}

float sigmoid(float x){
	return 1/(1+exp(-x));
}



float accuracy(int * prediction_labels,int * true_labels,int numOfLabels){

	float acc = 0.0;
	for(int i=0;i<numOfLabels;i++)
		acc += (prediction_labels[i] == true_labels[i] ? 1.0:0.0 );
	
	return ((float)(((float)acc)  / ((float)numOfLabels)))*100;
}

float euclid_norm(float * x, int size){
	float norm = 0.0;

	for (int i = 0; i < size; i++)	{
		norm+=x[i]*x[i];
	}

	return sqrt(norm);
}


float manhattan_norm(float * x, int size){
	float norm = 0.0;

	for (int i = 0; i < size; i++)	{
		norm+=x[i];
	}

	return norm;
}

HyperParameters * constructHyperParameters(
	float * learning_rates,
	int numofLr,
	float * threshold,
	int numofthreshold,
	int * max_epochs,
	int numOfmax_epochs
){


	HyperParameters * hp = malloc(sizeof(HyperParameters));
	hp->learning_rates   = learning_rates;
	hp->numofLr          = numofLr;
	hp->threshold        = threshold;
	hp->numofthreshold   = numofthreshold;
	hp->max_epochs       = max_epochs;
	hp->numOfmax_epochs  = numOfmax_epochs;


	return hp;
}


void GridSearch(Xy_Split * train,Xy_Split * test,HyperParameters * hp,size_t vectorSize,FILE * GridSearchFile){

	LogisticRegression * model;
	for(int me = 0; me<hp->numOfmax_epochs; me++){
		for(int lr=0;lr<hp->numofLr;lr++){
			for(int t=0;t<hp->numofthreshold;t++){

				if(GridSearchFile!=NULL) 
					fprintf(GridSearchFile, "\n ------ Lr %lf   |    threshold %lf  | max_epochs %d \n",hp->learning_rates[lr],hp->threshold[t],hp->max_epochs[me] );
				model  = LR_construct(vectorSize*2,hp->learning_rates[lr],hp->threshold[t],hp->max_epochs[me] );
				LR_fit(model,train);
				LR_Evaluation(model,test,GridSearchFile);
				LR_destroy(model);

			}
		}
	}
}

void LR_Evaluation(LogisticRegression * model,Xy_Split * eval_set,FILE * file){


	int * prediction_labels = malloc((eval_set->size)*sizeof(int));


	for (int j = 0; j < eval_set->size; j++){
		// printf("True label: %d | ",eval_set->y[j]);
		prediction_labels[j]  = LR_predict(model,eval_set->X[j],0);
		// printf("|  prediction:  %d \n ",prediction_labels[j]);
	}


	float acc = accuracy(prediction_labels,eval_set->y,eval_set->size);

	fprintf(file,"Accuracy %6.2lf %% \n",acc);

	free(prediction_labels);
}

void destroyHyperParameters(HyperParameters * hp){
	free(hp);
}