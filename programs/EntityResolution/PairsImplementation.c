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

List * printPairs(Clique** cliquesArray,int numOfsets ){

	FILE * output;
	List * sameCameras_list = createList();

	output = fopen("PAIRS.csv","w+");		// or in file
	fprintf(output, "left_spec_id, right_spec_id\n");
	for(int i=0;i<numOfsets;i++)	// for every spec
		printForward(cliquesArray[i]->set,output,printCameraName,sameCameras_list);	// print every pair in the list
	
	fclose(output);

	return sameCameras_list;
}

void printNegativePairs(List * diffPairs){

	FILE * output;

	output = fopen("NEGATIVE_PAIRS.csv","w+");		// or in file
	fprintf(output, "left_spec_id, right_spec_id\n");
	printForward(diffPairs,output,printCameraName,NULL);	// print every pair in the list
	
	fclose(output);
}

void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list){

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

		int cliqueKey_1 = pair->camera1->arrayPosition;
		int cliqueKey_2 = pair->camera2->arrayPosition;

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

List * createNegativePairs(Clique ** cliqueIndex,int numOfcliques,FILE * file){

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

Dataset * train_test_split_pairs(CamerasPair ** pairsArray,int * Labels,int datasetSize){

	Dataset * dataset = createDataset();


	for(int i=0;i<datasetSize;i++){

		float * concatedVectors = concatVectors(pairsArray[i]->camera1->vector,pairsArray[i]->camera2->vector,VectorSize);

		if(i < 0.6*datasetSize)

			dataset = insert_toDataset(dataset,concatedVectors,Labels[i],Train);

		else if(i >= 0.6*datasetSize && i < 0.8*datasetSize )

			dataset = insert_toDataset(dataset,concatedVectors,Labels[i],Test);

		else
			dataset = insert_toDataset(dataset,concatedVectors,Labels[i],Validation);


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
