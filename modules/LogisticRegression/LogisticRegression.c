#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <time.h>

#include "./../../include/LogisticRegression.h"


LogisticRegression* trainingModel;

DenseMatrix **   X_train;
int *  y_train;
float ** gradientsArray;

JobScheduler * scheduler;

void batchThread(void * args){

	int batch_first_element = ((threadArgs*)  args)->batch_first_element;
	int batch_last_element  = ((threadArgs*)  args)->batch_last_element;
	int gradient_position   =  ((threadArgs*)  args)->gradient_position;
	float loss,value;
	int weight_position;

	for(int i=batch_first_element; i<batch_last_element; i++){

		DenseMatrix * denseX = X_train[i];
		int denseX_size = denseX->matrixSize;
		
		if(denseX_size)
			loss =  LR_predict_proba(trainingModel,denseX) - y_train[i];


		for(int p=0; p<denseX_size; p++){

			weight_position = denseX->matrix[p]->position;
			value           = denseX->matrix[p]->value;
			gradientsArray[gradient_position][weight_position] += loss*value;

			// printf("%lf - %lf | ",loss,value);
		}
	}

	// for (int i = 0; i < trainingModel->vectorSize; i++){
	// 	printf("%lf -> ", gradientsArray[gradient_position][i]);
	// 	gradientsArray[gradient_position][i] /= ((float)(batch_last_element-batch_first_element));
	// 	printf("%lf |\n",gradientsArray[gradient_position][i] );
	// }
}


LogisticRegression* LR_construct(size_t vectorSize,float learning_rate,float threshold,int max_epochs,int batch_size,int numThreads){

	srand(time(NULL));

	LogisticRegression* model = malloc(sizeof(LogisticRegression));
	model->weights    = malloc(vectorSize*sizeof(float));
	model->vectorSize = vectorSize;
	model->learning_rate = learning_rate;
	model->threshold = threshold;
	model->max_epochs = max_epochs;
	model->batch_size = batch_size;
	model->numThreads = numThreads;
	
	for(int w=0;w<model->vectorSize;w++){
		// model->weights[w] = (rand()%2 == 0 ? -1:1)*1/rand();
		model->weights[w] = (float) ((float)1)/(float)rand();
		// model->weights[w] = 0.0;
	}

	model->bias = (float)((float)1)/rand();
	// model->bias = 0.0;
	// model->bias = 1.0;

	
	return model;
}

void LR_fit(LogisticRegression* model,Xy_Split * Xy_train){

	trainingModel = model;
	scheduler = initialize_scheduler(model->numThreads);
	int epochs=0;
	
	float * prev_weights = calloc(model->vectorSize,sizeof(float));
	int converged    = FALSE;
	X_train  = (DenseMatrix **) Xy_train->X;
	y_train          		= Xy_train->y;
	int N                   = Xy_train->size; 
	int batch_size = model->batch_size;
	int num_of_batches  = N/batch_size;
	int last_batch_size = N%batch_size;

	gradientsArray = malloc(sizeof(float*)*model->numThreads);
	for(int i=0;i<model->numThreads;i++)
		gradientsArray[i] = calloc(model->vectorSize,sizeof(float));

	
	while(!converged && epochs < model->max_epochs){
		
		batch_size = model->batch_size;
		num_of_batches  = N/batch_size;
		last_batch_size = N%batch_size;
		float prev_norm = euclid_norm(model->weights,model->vectorSize);
		int batch_first_element = 0;
		int batch_last_element = batch_first_element + batch_size;
		int current_batch = 0; 
		int submittedJobs=0;

		while(current_batch < num_of_batches+1){

			threadArgs * args = new_threadArgs(batch_first_element,batch_last_element,submittedJobs%model->numThreads);
			submit_job(scheduler,batchThread,(void*)args);
			submittedJobs++;

			if(submittedJobs % model->numThreads == 0 || current_batch == num_of_batches){

				int batches_executed = (submittedJobs % model->numThreads)+1;
				wait_activeJobs_finish(scheduler);

				float * avg_gradients = calloc(model->vectorSize,sizeof(float));

				for(int w=0;w<model->vectorSize;w++){
					for(int th=0;th<batches_executed;th++){
						// printf("%d %d %lf /",th,w,gradientsArray[th][w] );
						avg_gradients[w] += gradientsArray[th][w];
						gradientsArray[th][w] = 0;
					}
				}
					
				float avg_bias_gradient = 0;
				for(int w=0;w<model->vectorSize;w++){
					// printf("%lf %lf *** ",model->weights[w], ((float)(avg_gradients[w]*model->learning_rate))/(float)batches_executed);
					model->weights[w] = model->weights[w]-((float)(avg_gradients[w]*model->learning_rate))/((float)batches_executed);
					avg_bias_gradient += avg_gradients[w];
				}

				// model->bias -=  (float)((float)avg_bias_gradient*((float)model->learning_rate))/((float)batches_executed);
				
				free(avg_gradients);

			}
			
			current_batch++;
			if(current_batch < num_of_batches){
				batch_first_element += batch_size;
				batch_last_element = batch_first_element + batch_size;
			}else{
				batch_first_element += batch_size;
				batch_last_element += last_batch_size;
			}

		}

		float new_norm = euclid_norm(model->weights,model->vectorSize);

		// printf("| %lf | ", fabs(new_norm - prev_norm) );
		fflush(stdout);
		if(fabs(new_norm - prev_norm) < model->threshold){
			printf("Converged in %d epochs\n",epochs );
			// free(gradient);
			break;
		}

		epochs++;
	}

	destroy_scheduler(scheduler);
	for(int i=0;i<model->numThreads;i++)
		free(gradientsArray[i]);
	free(gradientsArray);
	
	free(prev_weights);
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
	return 1/(float)(1+exp(-x));
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
	int numOfmax_epochs,
	int batch_size,
	int numThreads
){


	HyperParameters * hp = malloc(sizeof(HyperParameters));
	hp->learning_rates   = learning_rates;
	hp->numofLr          = numofLr;
	hp->threshold        = threshold;
	hp->numofthreshold   = numofthreshold;
	hp->max_epochs       = max_epochs;
	hp->numOfmax_epochs  = numOfmax_epochs;
	hp->batch_size       = batch_size;
	hp->numThreads		 = numThreads;


	return hp;
}


void GridSearch(Xy_Split * train,Xy_Split * test,HyperParameters * hp,size_t vectorSize,FILE * GridSearchFile){

	LogisticRegression * model;
	for(int me = 0; me<hp->numOfmax_epochs; me++){
		for(int lr=0;lr<hp->numofLr;lr++){
			for(int t=0;t<hp->numofthreshold;t++){

				if(GridSearchFile!=NULL) 
					fprintf(GridSearchFile, "\n ------ Lr %lf   |    threshold %lf  | max_epochs %d \n",hp->learning_rates[lr],hp->threshold[t],hp->max_epochs[me] );
				model  = LR_construct(vectorSize*2,hp->learning_rates[lr],hp->threshold[t],hp->max_epochs[me],hp->batch_size,hp->numThreads );
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
		printf("True label: %d | ",eval_set->y[j]);
		prediction_labels[j]  = LR_predict(model,eval_set->X[j],1);
		printf("|  prediction:  %d \n ",prediction_labels[j]);
	}


	float acc = accuracy(prediction_labels,eval_set->y,eval_set->size);

	fprintf(file,"Accuracy %6.2lf %% \n",acc);

	free(prediction_labels);
}

void destroyHyperParameters(HyperParameters * hp){
	free(hp);
}

threadArgs * new_threadArgs(int batch_first_element,int batch_last_element,int gradient_position){

	threadArgs * args = malloc(sizeof(threadArgs));
	args->batch_first_element = batch_first_element;
	args->batch_last_element = batch_last_element;
	args->gradient_position = gradient_position;
	return args;
}