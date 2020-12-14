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



Clique** CreateSets(DisJointSet * djSet,int* numOfsets){
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

void destroySets(Clique** cliquesArray,int numOfsets){
	free(cliquesArray);
}



void train_test_split(Clique ** cliqueIndex,int numOfCliques){

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
				float * X       = camera->vector;

				if(l == numOfNegativeCliques) y=1;
				else y=0;


				if(currdata_inSet[l] <= 0.6*alldata_inSet[l])
					dataset = insert_toDataset(dataset,X,y,Train);
				else if(currdata_inSet[l] > 0.6*alldata_inSet[l] && currdata_inSet[l] <= 0.8*alldata_inSet[l])
					dataset = insert_toDataset(dataset,X,y,Test);
				else
					dataset = insert_toDataset(dataset,X,y,Validation);


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



void trainCliques(Clique** cliqueIndex,int numOfCliques,float learning_rate,float threshold){

	Clique* clique;
	Xy_Split * train;
	int max_epochs =10;
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
	int prediction;
	float ** X_test;
	int * y_test;
	int * prediction_labels;
	float acc = 0.0;
	float * accuracyArray = malloc(numOfCliques*sizeof(float));

	for(int i=0; i<numOfCliques; i++){

		clique = cliqueIndex[i];
		test = clique->dataset->test;
		X_test = test->X;
		y_test = test->y;

		prediction_labels =  malloc((test->size)*sizeof(int));

		for (int j = 0; j < test->size; j++){
			printf("True label: %d | ",y_test[j]);
			prediction = LR_predict(clique->LRModel,X_test[j],1);
			prediction_labels[j] = prediction;
			printf("|  prediction:  %d \n ",prediction);
		}
		printf("\n\n");
		

		acc =  accuracy(prediction_labels,y_test,test->size);

		accuracyArray = realloc(accuracyArray,(i+1)*sizeof(float));
		accuracyArray[i] = acc;	
		
		free(prediction_labels);	
	}

	return accuracyArray;	
}