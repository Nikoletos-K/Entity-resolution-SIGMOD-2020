#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "./../../include/Clique.h"
#include "./Vectorization.h"


Clique** CreateCliques(DisJointSet * djSet,int* numOfsets){
	int parent;
	Clique** cliquesArray = malloc(sizeof(Clique*));
	List* set;
	CamSpec* data;
	
	CamSpec** camArray = (CamSpec**) (djSet->objectArray);

	for(int i=0;i<djSet->size;i++){
		parent = DJSFindParent(djSet,i);
		if(parent!=i)
			insert_toList(camArray[parent]->set,camArray[i]);
	}

	for(int i=0;i<djSet->size;i++){	// for every spec
		if(!oneNodeList(camArray[i]->set)){		
			cliquesArray = realloc(cliquesArray,(*numOfsets+1)*sizeof(Clique*));
			cliquesArray[*numOfsets] = malloc(sizeof(Clique));
			cliquesArray[*numOfsets]->set = camArray[i]->set;
			cliquesArray[*numOfsets]->numOfNegativeCliques = 0;
			cliquesArray[*numOfsets]->negativeCliques = NULL;
			cliquesArray[*numOfsets]->numOfUnique_negativeCliques = 0;
			cliquesArray[*numOfsets]->unique_negativeCliques = NULL;
						
			cliquesArray[*numOfsets]->LRModel = NULL;
			cliquesArray[*numOfsets]->dataset = NULL;

			(*numOfsets)++;

			set = (cliquesArray[*numOfsets-1])->set;

			listNode * node = set->firstNode;
	
			while(node!=NULL){

				data = (CamSpec*) node->data;
				data->arrayPosition = *numOfsets-1;
				node = node->nextNode;
			}
		}	
	}

	for (int i = 0; i < *numOfsets; i++)
		cliquesArray[i]->bitArray =  createBF(*numOfsets);
	
	return cliquesArray;
}

void destroyCliques(Clique** cliquesArray,int numOfCliques){

	for (int i = 0; i < numOfCliques; i++){
		if(cliquesArray[i]->dataset != NULL)
			destroy_Dataset(cliquesArray[i]->dataset);
		destroyBF(cliquesArray[i]->bitArray);
		if(cliquesArray[i]->LRModel!=NULL)
			LR_destroy(cliquesArray[i]->LRModel);
		free(cliquesArray[i]->unique_negativeCliques);
		free(cliquesArray[i]->negativeCliques);
		free(cliquesArray[i]);
	}
	free(cliquesArray);
}

void train_test_split_Cliques(Clique ** cliqueIndex,int numOfCliques){

	for(int c=0;c<numOfCliques;c++){

		Dataset * dataset = createDataset();
		List * clique    = cliqueIndex[c]->set;
		int sizeofClique = get_listSize(clique);		
		int numOfNegativeCliques = cliqueIndex[c]->numOfNegativeCliques;
		int * currdata_inSet    = calloc(numOfNegativeCliques+1,sizeof(int));
		int * alldata_inSet = calloc(numOfNegativeCliques+1,sizeof(int));

		listNode ** nodesRead = malloc(sizeof(listNode*)*(numOfNegativeCliques+1));
	
		for(int i=0;i<numOfNegativeCliques;i++){
			int negClique_pos = cliqueIndex[c]->negativeCliques[i];
			nodesRead[i] = cliqueIndex[negClique_pos]->set->firstNode;
			alldata_inSet[i] = get_listSize(cliqueIndex[negClique_pos]->set);
		}

		nodesRead[numOfNegativeCliques] = clique->firstNode;
		alldata_inSet[numOfNegativeCliques] = sizeofClique;


		int setsCreated = FALSE,y;

		while(!setsCreated){

			for(int l=0;l<numOfNegativeCliques+1;l++){

				if(nodesRead[l] == NULL)
					continue;


				CamSpec* camera = (CamSpec*)nodesRead[l]->data;
<<<<<<< Updated upstream
				DenseMatrix * X       = camera->DenseVector;
=======
				DenseMatrix * X = camera->DenseVector;
>>>>>>> Stashed changes

				if(l == numOfNegativeCliques) y=1;
				else y=0;


				if(currdata_inSet[l] <= 0.6*alldata_inSet[l])
					dataset = insert_toDataset(dataset,X,y,Train);
				else if(currdata_inSet[l] > 0.6*alldata_inSet[l] && currdata_inSet[l] <= 0.8*alldata_inSet[l])
					dataset = insert_toDataset(dataset,X,y,Test);
				else
					dataset = insert_toDataset(dataset,X,camera->arrayPosition,Validation);


				currdata_inSet[l]++;
				nodesRead[l] = nodesRead[l]->nextNode;
			}


			int listsFinished=TRUE;
			for(int l=0;l<numOfNegativeCliques+1;l++){
				if(nodesRead[l] != NULL)
					listsFinished = FALSE;
			}

			if(listsFinished==TRUE) break;
		}
		
		cliqueIndex[c]->dataset = dataset;

		free(currdata_inSet);
		free(alldata_inSet);
		free(nodesRead);
	}
}


void trainCliques(Clique** cliqueIndex,int numOfCliques,float learning_rate,float threshold,int max_epochs){

	Clique* clique;
	Xy_Split * train;

	for(int i=0; i<numOfCliques; i++){

		clique = cliqueIndex[i];
		train = clique->dataset->train;

		clique->LRModel = LR_construct(VectorSize,learning_rate,threshold,max_epochs);
		LR_fit(clique->LRModel,train);
	}
}



float* testCliques(Clique** cliqueIndex,int numOfCliques){

	Clique* clique;
	Xy_Split * test;
	int * prediction_labels;
	float * accuracyArray = malloc(numOfCliques*sizeof(float));

	for(int i=0; i<numOfCliques; i++){

		clique = cliqueIndex[i];
		test   = clique->dataset->test;

		prediction_labels =  malloc((test->size)*sizeof(int));

		for (int j = 0; j < test->size; j++){
			printf("True label: %d | ",test->y[j]);
			prediction_labels[j] = LR_predict(clique->LRModel,test->X[j],1);
			printf("|  prediction:  %d \n ",prediction_labels[j]);
		}
		printf("\n\n");
		
		accuracyArray[i] =  accuracy(prediction_labels,test->y,test->size);
		
		free(prediction_labels);	
	}

	return accuracyArray;	
}


float* validateCliques(Clique** cliqueIndex,int numOfCliques){

	Clique* clique;
	Xy_Split * validation;
	float prediction;
	float * accuracyArray = malloc(numOfCliques*sizeof(float));
	float max_prediction;
	int max_clique;
	int numOfCorrectPred;

	for(int i=0; i<numOfCliques; i++){

		clique = cliqueIndex[i];
		validation = clique->dataset->validation;
		numOfCorrectPred = 0;
		
		for (int j = 0; j < validation->size; j++){
			max_prediction = 0.0;
			for(int k=0; k<numOfCliques; k++){
				clique = cliqueIndex[k];
				prediction =  LR_predict_proba(clique->LRModel,validation->X[j]);
				printf("clique: %d | prediction: %lf \n ",k,prediction );
				if(prediction > max_prediction){
					max_prediction = prediction;
					max_clique = k;
				}
			}
			if(max_clique == validation->y[j]){
				numOfCorrectPred++;
			}
			printf("---> correct:  %d | predict:  %d \n",validation->y[j],max_clique );
		}

		accuracyArray[i] = numOfCorrectPred/ (float) validation->size;		
	}

	return accuracyArray;	
}