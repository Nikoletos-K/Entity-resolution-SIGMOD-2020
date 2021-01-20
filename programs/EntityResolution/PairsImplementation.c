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

#include "./../../include/PairsImplementation.h"

retraining_set** retrainingArray;
int* num_of_retrain_specs;

JobScheduler * pairsScheduler;

List * printPairs(Clique** cliquesArray,int numOfsets,int num_of_cameras ){

	FILE * output;
	List * sameCameras_list = createList();

	output = fopen("PAIRS.csv","w+");		// or in file
	fprintf(output, "left_spec_id, right_spec_id\n");
	for(int i=0;i<numOfsets;i++)	// for every spec
		printForward(cliquesArray[i]->set,output,printCameraName,sameCameras_list,num_of_cameras);	// print every pair in the list
	
	fclose(output);

	return sameCameras_list;
}

void printNegativePairs(List * diffPairs,int num_of_cameras){

	FILE * output;

	output = fopen("NEGATIVE_PAIRS.csv","w+");		// or in file
	fprintf(output, "left_spec_id, right_spec_id\n");
	printForward(diffPairs,output,printCameraName,NULL,num_of_cameras);	// print every pair in the list
	
	fclose(output);
}

void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list,int num_of_cameras){

	listNode * leftNode = list->firstNode,* rightNode;
	
	while(leftNode!=NULL){

		rightNode = leftNode->nextNode;
		while(rightNode!=NULL){

			printData(leftNode->data,output);
			fprintf(output,", ");
			printData(rightNode->data,output);
			fprintf(output,"\n");

			if(sameCameras_list!=NULL){	
				CamerasPair * pair = createPair((CamSpec *) leftNode->data, (CamSpec *) rightNode->data);
				insert_toList(sameCameras_list,(void*) pair);
				if(pair->camera1->bitArray!=NULL && pair->camera2->bitArray!=NULL){
					setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
					setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
				}else if(pair->camera1->bitArray==NULL && pair->camera2->bitArray!=NULL){
					pair->camera1->bitArray = createBF(num_of_cameras);
					setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
					setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
				}else if(pair->camera2->bitArray==NULL && pair->camera1->bitArray!=NULL){
					pair->camera2->bitArray = createBF(num_of_cameras);
					setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
					setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
				}
			}
			rightNode = rightNode->nextNode;
		}
		leftNode = leftNode->nextNode;
	}
}



Clique** createNegConnections(List * diffPairsList,Clique ** cliqueIndex){

	listNode * node = diffPairsList->firstNode;
	while(node != NULL){

		CamerasPair * pair = (CamerasPair*) node->data;

		int cliqueKey_1 = pair->camera1->myClique;
		int cliqueKey_2 = pair->camera2->myClique;

		if(!checkBit(cliqueIndex[cliqueKey_1]->bitArray,cliqueKey_2)){
			setBit(cliqueIndex[cliqueKey_1]->bitArray,cliqueKey_2);
			cliqueIndex[cliqueKey_1] = insert_NegConnection(cliqueIndex[cliqueKey_1],cliqueKey_2);
			cliqueIndex[cliqueKey_1] = insert_uniqueNegConnection(cliqueIndex[cliqueKey_1],cliqueKey_2);
		}

		if(!checkBit(cliqueIndex[cliqueKey_2]->bitArray,cliqueKey_1)){
			setBit(cliqueIndex[cliqueKey_2]->bitArray,cliqueKey_1);
			cliqueIndex[cliqueKey_2] = insert_NegConnection(cliqueIndex[cliqueKey_2],cliqueKey_1);	
		}

		node = node->nextNode;
	}

	return cliqueIndex;
}


Clique * insert_NegConnection(Clique * cl,int arrayPosition){

	if(cl->negativeCliques == NULL)
		cl->negativeCliques = malloc(sizeof(int));
	
	else
		cl->negativeCliques = realloc(cl->negativeCliques,(cl->numOfNegativeCliques+1)*sizeof(int));
	

	cl->negativeCliques[cl->numOfNegativeCliques] = arrayPosition;
	(cl->numOfNegativeCliques)++;

	return cl;
}

Clique * insert_uniqueNegConnection(Clique * cl,int arrayPosition){

	if(cl->unique_negativeCliques == NULL)
		cl->unique_negativeCliques = malloc(sizeof(int));
	
	else
		cl->unique_negativeCliques = realloc(cl->unique_negativeCliques,(cl->numOfUnique_negativeCliques+1)*sizeof(int));
	

	cl->unique_negativeCliques[cl->numOfUnique_negativeCliques] = arrayPosition;
	(cl->numOfUnique_negativeCliques)++;

	return cl;
}

CamerasPair * createPair(CamSpec * c1, CamSpec * c2){

	CamerasPair * pair = malloc(sizeof(CamerasPair));
	pair-> camera1 = c1;
	pair-> camera2 = c2;
	pair-> trueLabel = -1;

	return pair;

}

List * createNegativePairs(Clique ** cliqueIndex,int numOfcliques,FILE * file, int num_of_cameras ){

	List * differentCameras = createList();
	int negativeClique_position=-1;

	for(int i=0;i<numOfcliques;i++){

		List * Clique = cliqueIndex[i]->set;
		listNode * clique_node = Clique->firstNode;

		while(clique_node != NULL){
	
			for(int c=0;  c<cliqueIndex[i]->numOfUnique_negativeCliques;  c++){

				negativeClique_position = cliqueIndex[i]->unique_negativeCliques[c];

				List * NegClique = cliqueIndex[negativeClique_position]->set;
				listNode * negclique_node = NegClique->firstNode;

				while(negclique_node != NULL){

					CamerasPair * pair = createPair((CamSpec*)clique_node->data,(CamSpec*)negclique_node->data);

					insert_toList(differentCameras ,(void*)pair);
					if(pair->camera1->bitArray!=NULL && pair->camera2->bitArray!=NULL){
						setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
						setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
					}else if(pair->camera1->bitArray==NULL && pair->camera2->bitArray!=NULL){
						pair->camera1->bitArray = createBF(num_of_cameras);
						setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
						setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
					}else if(pair->camera2->bitArray==NULL && pair->camera1->bitArray!=NULL){
						pair->camera2->bitArray = createBF(num_of_cameras);
						setBit(pair->camera1->bitArray,pair->camera2->arrayPosition);
						setBit(pair->camera2->bitArray,pair->camera1->arrayPosition);
					}
					negclique_node = negclique_node->nextNode;
					fprintf(file,"%s, %s \n",pair->camera1->name,pair->camera2->name);
				}
			}
			clique_node = clique_node->nextNode;
		}
	}

	return differentCameras;
}

/* ---------------------- Dataset ---------------------- */

CamerasPair ** create_PairsDataset(List * sameCameras,List * differentCameras,int * Labels,int dataset_size,int stratify){

	srand(time(NULL));


	CamerasPair ** Dataset = malloc(dataset_size*sizeof(CamerasPair *));

	int i=0,isertAlteranety=TRUE;

	listNode * samePair_node = sameCameras->firstNode;
	listNode * first_difPair_node = differentCameras->firstNode;
	listNode * last_difPair_node = differentCameras->lastNode;

	

	while(first_difPair_node!=last_difPair_node || samePair_node!=NULL){

		if(first_difPair_node!=last_difPair_node){
			setLabel(first_difPair_node->data,0);
			setLabel(last_difPair_node->data,0);
		}


		if( (i%stratify == 0 && samePair_node!=NULL) || (first_difPair_node==last_difPair_node && samePair_node !=NULL)){
		
			if(samePair_node!=NULL)	
				setLabel(samePair_node->data,1);

			Dataset[i] = (CamerasPair*)samePair_node->data;
			Labels[i] = 1;
			i++;

			if(samePair_node!=NULL)
				samePair_node = samePair_node->nextNode;
		

		}else if(first_difPair_node!=last_difPair_node){

			if(isertAlteranety){

				Dataset[i] = (CamerasPair*) first_difPair_node->data;
				Labels[i] = 0;
				i++;	

				first_difPair_node = first_difPair_node->nextNode;
			
				if(first_difPair_node == last_difPair_node){
					Dataset[i] = (CamerasPair*) first_difPair_node->data;
					Labels[i] = 0;
					i++;	
				}
				isertAlteranety = FALSE;
			
			}else{
		
				Dataset[i] = (CamerasPair*) last_difPair_node->data;
				Labels[i] = 0;
				i++;	
				last_difPair_node = last_difPair_node->prevNode;
				if(first_difPair_node == last_difPair_node){
					Dataset[i] = (CamerasPair*) last_difPair_node->data;
					Labels[i] = 0;
					i++;					
				}
			}
			isertAlteranety = TRUE;
		}
	}

	return Dataset;
}


Dataset * train_test_split_pairs(CamerasPair ** pairsArray,int * Labels,int datasetSize,int stratify){

	Dataset * dataset = createDataset();

	srand(time(NULL));
	// int random_seed = rand();

	int validationItems = 0;
	int trainItems      = 0;
	int testItems       = 0;
	int allItems        = 0,i=0,insertAlternatelly=0,stratifyCounter=0;

	while(allItems < datasetSize){

		DenseMatrix * concatedDenseVector = concatDenseMatrices(pairsArray[i]->camera1->DenseVector,pairsArray[i]->camera2->DenseVector,VectorSize);

		if (concatedDenseVector==NULL){
			i++;
			continue;
		}
		
		if( insertAlternatelly == 0 && trainItems<0.6*datasetSize) {
		
			dataset = insert_toDataset(dataset,concatedDenseVector,Labels[i],Train);
			trainItems++;
			if(trainItems == 0.6*datasetSize)
				printf("- Train set READY \n");
			i++;
			stratifyCounter++;

		}else if( insertAlternatelly == 1 && testItems<0.2*datasetSize) {
		
			dataset = insert_toDataset(dataset,concatedDenseVector,Labels[i],Test);
			testItems++;
			i++;
			stratifyCounter++;

			if(testItems == 0.2*datasetSize)
				printf("- Test set READY \n");

		}else if( insertAlternatelly==2  && validationItems<0.2*datasetSize ) {
			dataset = insert_toDataset(dataset,concatedDenseVector,Labels[i],Validation);
			validationItems++;
			i++;
			stratifyCounter++;
		
			if(validationItems == 0.2*datasetSize)
				printf("- Validation set READY \n");

		}else{
			destroyDenseMatrix(concatedDenseVector);
			insertAlternatelly = (insertAlternatelly+1)%3;
			stratifyCounter=0;
		}


		if(stratifyCounter == stratify){
			
			if(validationItems == 0.2*datasetSize && testItems == 0.2*datasetSize)
				insertAlternatelly = 0;
			else
				insertAlternatelly = (insertAlternatelly+1)%3;


			stratifyCounter=0; 
		}
		allItems = trainItems+ testItems + validationItems;
	}

	return dataset;

}

void printDataset(CamerasPair ** Dataset,int dataset_size){

	FILE * dataset_file = fopen("DATASET.csv","w+");
	for(int i=0;i<dataset_size;i++)
		fprintf(dataset_file, "%s, %s, %d,  \n",((CamSpec*)Dataset[i]->camera1)->name,((CamSpec*)Dataset[i]->camera2)->name, Dataset[i]->trueLabel);

	fclose(dataset_file);
}

void setLabel(CamerasPair *  pair,int label){
	pair->trueLabel = label;
}

void deletePair(CamerasPair * pair){
	free(pair);
}

void createVectors(CamSpec ** camArray,int num_of_cameras){

	printf("Dictionary size: %lu\n",DictionarySize );


	for(int p=0;p<DictionarySize;p++){
		float idf = log(num_of_cameras/DictionaryNodes[p]->jsonsIn);
		float tfs = DictionaryNodes[p]->sumOfTfs;
		DictionaryNodes[p]->averageTfIdf = (tfs * idf)/(float) num_of_cameras;
	}
	

	qsort(DictionaryNodes, DictionarySize, sizeof(dictNode*), compareAverageTfIdf);


	int * dictionaryMap = calloc(DictionarySize,sizeof(int));
	float tf,idf;
	int mapIndex = 1;
	for (int i = DictionarySize-1; i >= DictionarySize-VectorSize; i--){
		int position  = DictionaryNodes[i]->wordID; 
		// printf("%lf %d %s\n",DictionaryNodes[i]->averageTfIdf,DictionaryNodes[i]->jsonsIn,DictionaryNodes[i]->word );
		dictionaryMap[position] = mapIndex;
		mapIndex++;
	}

	for(int i=0;i<num_of_cameras;i++){

		int numOfWords       = camArray[i]->numOfWords;
		int* dictionaryWords = camArray[i]->dictionaryWords;
		int* wordCounters = camArray[i]->wordCounters;

		if (numOfWords>0){
		
			camArray[i] -> DenseVector = createDenseMatrix();


			for(int p=0;p<numOfWords;p++){
				
				int vector_position = dictionaryWords[p];
				int final_vector_position = dictionaryMap[vector_position];

				if(final_vector_position!=0){
					tf = wordCounters[p]/(float)numOfWords;
					idf = log(num_of_cameras/DictionaryNodes[vector_position]->jsonsIn);
					camArray[i] -> DenseVector = DenseMatrix_insert(camArray[i] -> DenseVector,tf*idf,final_vector_position-1);
					// printf("## %d\n", final_vector_position-1);			
				}
			}
		
		}else
			camArray[i] -> DenseVector = NULL;
		
	}
	free(dictionaryMap);
}

void create_retrainScheduler(int numThreads){
	pairsScheduler = initialize_scheduler(numThreads);
}


void destroy_retrainScheduler(){
	destroy_scheduler(pairsScheduler);
}


retraining_set ** LR_retrain(retraining_set** retrainingArray_in,LogisticRegression* model,CamSpec ** camArray, int num_of_cameras, float threshold,int* num_of_retrain_specs_in,size_t VectorSize){

	// float prediction = 0.0;

	retrainingArray = retrainingArray_in;
	num_of_retrain_specs = num_of_retrain_specs_in;

	num_of_cameras = num_of_cameras/10;

	for (int i = 0; i < num_of_cameras; i++){

		pairsArgs * args = new_pairsArgs(model,camArray,num_of_cameras,threshold,VectorSize,i);
		
		submit_job(pairsScheduler,update_retrainArray,(void*)args);


		// int j = i+1;
		// while (j < num_of_cameras && (camArray[i]->bitArray == NULL || (camArray[i]->bitArray != NULL && !checkBit(camArray[i]->bitArray,j)))){

		// 	DenseMatrix * concatedVector = concatDenseMatrices(camArray[i]->DenseVector,camArray[j]->DenseVector,VectorSize);
			
		// 	int denseX_size = concatedVector->matrixSize;
	
		// 	if(denseX_size){
		// 		prediction =  LR_predict_proba(model,concatedVector);

		// 		if(prediction < threshold || prediction> (1-threshold)){
		// 			retrainingArray = realloc(retrainingArray,(*num_of_retrain_specs+1)*sizeof(retraining_set*));
		// 			retrainingArray[*num_of_retrain_specs] = malloc(sizeof(retraining_set));
		// 			retrainingArray[*num_of_retrain_specs]->camera1 = camArray[i];
		// 			retrainingArray[*num_of_retrain_specs]->camera2 = camArray[j];
		// 			retrainingArray[*num_of_retrain_specs]->concatedVector = concatedVector;
		// 			retrainingArray[*num_of_retrain_specs]->prediction = prediction;
		// 			// printf("%d. %s - %s ---> %lf\n",*num_of_retrain_specs, retrainingArray[*num_of_retrain_specs]->camera1->name, retrainingArray[*num_of_retrain_specs]->camera2->name, retrainingArray[*num_of_retrain_specs]->prediction);					
		// 			(*num_of_retrain_specs)++;
		// 		}else
		// 			destroyDenseMatrix(concatedVector);
				
		// 	}else
		// 		destroyDenseMatrix(concatedVector);

		// 	j+=100;
		// }
	}

	wait_activeJobs_finish(pairsScheduler);
	return retrainingArray;
}

void update_retrainArray(void * default_args){

	
	pairsArgs * args = (pairsArgs*) default_args;

	LogisticRegression* model = (LogisticRegression*) args->model;
	CamSpec ** camArray = (CamSpec **) args->camArray;
	int num_of_cameras = (int) args->num_of_cameras;
	float threshold = (float) args->threshold;
	size_t VectorSize = (size_t) args->VectorSize;
	int i = (int) args->i;
	float prediction = 0.0;
	int j = i+1;

	while (j < num_of_cameras && (camArray[i]->bitArray == NULL || (camArray[i]->bitArray != NULL && !checkBit(camArray[i]->bitArray,j)))){

		DenseMatrix * concatedVector = concatDenseMatrices(camArray[i]->DenseVector,camArray[j]->DenseVector,VectorSize);
		
		int denseX_size = concatedVector->matrixSize;

		if(denseX_size){
			prediction =  LR_predict_proba(model,concatedVector);

			if(prediction < threshold || prediction > (1-threshold)){
				pthread_mutex_lock(&(pairsScheduler->queue_mtx));
				retrainingArray = realloc(retrainingArray,(*num_of_retrain_specs+1)*sizeof(retraining_set*));
				retrainingArray[*num_of_retrain_specs] = malloc(sizeof(retraining_set));
				retrainingArray[*num_of_retrain_specs]->camera1 = camArray[i];
				retrainingArray[*num_of_retrain_specs]->camera2 = camArray[j];
				retrainingArray[*num_of_retrain_specs]->concatedVector = concatedVector;
				retrainingArray[*num_of_retrain_specs]->prediction = prediction;
				// printf("%d. %s - %s ---> %lf\n",*num_of_retrain_specs, retrainingArray[*num_of_retrain_specs]->camera1->name, retrainingArray[*num_of_retrain_specs]->camera2->name, retrainingArray[*num_of_retrain_specs]->prediction);					
				(*num_of_retrain_specs)++;
				pthread_mutex_unlock(&(pairsScheduler->queue_mtx));
			}else
				destroyDenseMatrix(concatedVector);
			
		}else
			destroyDenseMatrix(concatedVector);

		j+=100;
	}

}


Xy_Split * resolve_transitivity_issues(Xy_Split * Xy_train,Clique*** cliqueIndex,int * numOfCliques,retraining_set** retrainingArray, int num_of_retrain_specs,int num_of_cameras, float threshold){

	retraining_set* retrained_pair;
	CamSpec * camera1;
	CamSpec * camera2;
	

	for (int i = 0; i < num_of_retrain_specs; i++){
		retrained_pair = retrainingArray[i];
		camera1 = retrained_pair->camera1;
		camera2 = retrained_pair->camera2;

		if(camera1->myClique != -1 &&  camera2->myClique != -1){
			if(!compareCliques(*cliqueIndex,camera1->myClique,camera2->myClique)){
				if(retrained_pair->prediction < threshold){

					int cliqueKey_1 = camera1->myClique;
					int cliqueKey_2 = camera2->myClique;

					if(!checkBit((*cliqueIndex)[cliqueKey_1]->bitArray,cliqueKey_2)){

						setBit((*cliqueIndex)[cliqueKey_1]->bitArray,cliqueKey_2);
						(*cliqueIndex)[cliqueKey_1] = insert_NegConnection((*cliqueIndex)[cliqueKey_1],cliqueKey_2);
						(*cliqueIndex)[cliqueKey_1] = insert_uniqueNegConnection((*cliqueIndex)[cliqueKey_1],cliqueKey_2);
					}

					if(!checkBit((*cliqueIndex)[cliqueKey_2]->bitArray,cliqueKey_1)){
						setBit((*cliqueIndex)[cliqueKey_2]->bitArray,cliqueKey_1);
						(*cliqueIndex)[cliqueKey_2] = insert_NegConnection((*cliqueIndex)[cliqueKey_2],cliqueKey_1);	
					}

					Xy_train = insert_toXy_Train(Xy_train,retrained_pair->concatedVector,0);
					retrained_pair->concatedVector = NULL;

				}else{
					Clique * clique1 =  (*cliqueIndex)[camera1->myClique];
					Clique * clique2 =  (*cliqueIndex)[camera2->myClique];
					clique1->set =  mergeLists(clique1->set, clique2->set);
					camera2->myClique = camera1->myClique;

					Xy_train = insert_toXy_Train(Xy_train,retrained_pair->concatedVector,1);
					retrained_pair->concatedVector = NULL;			
				}

				if(camera1->bitArray!=NULL && camera2->bitArray!=NULL){
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera1->bitArray==NULL && camera2->bitArray!=NULL){
					camera1->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera2->bitArray==NULL && camera1->bitArray!=NULL){
					camera2->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}
			}

		}else if(camera1->myClique != -1 &&  camera2->myClique == -1){

			Clique * clique1 =  (*cliqueIndex)[camera1->myClique];

			if(retrained_pair->prediction > (1-threshold)){
				insert_toList(clique1->set,camera2);
				camera2->myClique = camera1->myClique;

				if(camera1->bitArray!=NULL && camera2->bitArray!=NULL){
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera1->bitArray==NULL && camera2->bitArray!=NULL){
					camera1->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera2->bitArray==NULL && camera1->bitArray!=NULL){
					camera2->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}

				Xy_train = insert_toXy_Train(Xy_train,retrained_pair->concatedVector,1);
				retrained_pair->concatedVector = NULL;			

			}
		
		}else if(camera1->myClique == -1 &&  camera2->myClique != -1){
			
			Clique * clique2 =  (*cliqueIndex)[camera2->myClique];

			if(retrained_pair->prediction > (1-threshold)){
				insert_toList(clique2->set,camera2);
				camera1->myClique = camera2->myClique;

				if(camera1->bitArray!=NULL && camera2->bitArray!=NULL){
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera1->bitArray==NULL && camera2->bitArray!=NULL){
					camera1->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}else if(camera2->bitArray==NULL && camera1->bitArray!=NULL){
					camera2->bitArray = createBF(num_of_cameras);
					setBit(camera1->bitArray,camera2->arrayPosition);
					setBit(camera2->bitArray,camera1->arrayPosition);
				}

				Xy_train = insert_toXy_Train(Xy_train,retrained_pair->concatedVector,1);
				retrained_pair->concatedVector = NULL;			
			}
		}else if(camera1->myClique == -1 &&  camera2->myClique == -1){
			if(retrained_pair->prediction > (1-threshold)){
				Clique * newClique =  constructClique();
				newClique = addToClique(newClique, camera1);
				newClique = addToClique(newClique, camera2);

				*cliqueIndex = realloc(*cliqueIndex,(*numOfCliques+1)*sizeof(Clique*));
				(*cliqueIndex)[*numOfCliques] = newClique;
				(*numOfCliques)++;

				Xy_train = insert_toXy_Train(Xy_train,retrained_pair->concatedVector,1);
				retrained_pair->concatedVector = NULL;	
			}

		}		
	}
	return Xy_train;
}

void destroyRetrainArray(retraining_set** retrainingArray, int size){
	
	for (int i = 0; i < size; i++){
		if(retrainingArray[i]->concatedVector != NULL){
			destroyDenseMatrix(retrainingArray[i]->concatedVector);
			retrainingArray[i]->concatedVector = NULL;	
		}
		free(retrainingArray[i]);
	}

	free(retrainingArray);
}

int compareRetrainingSet(const void * a, const void * b){

	retraining_set * nodeA = *(retraining_set**) a;
	retraining_set * nodeB = *(retraining_set**) b;

	float diff = (((float)nodeA->prediction - (float)nodeB->prediction));
   	
   	if(diff>0.0) diff=-1;
   	else if(diff== 0.0) diff=0;
   	else diff = 1;

   return diff;
}

pairsArgs * new_pairsArgs(	LogisticRegression* model,CamSpec ** camArray,int num_of_cameras,float threshold,size_t VectorSize,int i){
	
	pairsArgs * temp = malloc(sizeof(pairsArgs));
	temp->	model = model;
	temp->camArray = camArray;
	temp->num_of_cameras = num_of_cameras;
	temp->threshold = threshold;
	temp->VectorSize = VectorSize;
	temp->i = i;

	return temp;
}