#pragma once

#include "./Clique.h"
// #include "./Vectorization.h"
// #include "./LogisticRegression.h"

typedef struct CamerasPair{
	
	CamSpec * camera1;
	CamSpec * camera2;
	int trueLabel;

} CamerasPair;

typedef struct retraining_set{
	CamSpec * camera1;
	CamSpec * camera2;
	DenseMatrix * concatedVector;
	float prediction;
}retraining_set;

typedef struct pairsArgs {
	
	LogisticRegression* model;
	CamSpec ** camArray;
	int num_of_cameras;
	float threshold;
	size_t VectorSize;
	int i;

}pairsArgs;



List * printPairs(Clique** setsList,int numOfsets,int num_of_cameras );
void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list,int num_of_cameras);

CamerasPair * createPair(CamSpec * c1, CamSpec * c2);
void deletePair(CamerasPair * pair);
void setLabel(CamerasPair *  pair,int label);
Clique** createNegConnections(List * diffPairsList,Clique ** CliqueIndex);
Clique * insert_uniqueNegConnection(Clique * cl,int arrayPosition);
Clique * insert_NegConnection(Clique * ql,int arrayPosition);
List * createNegativePairs(Clique ** CliqueIndex,int numOfcliques,FILE * file, int num_of_cameras);

CamerasPair ** create_PairsDataset(List * sameCameras,List * differentCameras,int * Labels,int dataset_size,int stratify);
void printDataset(CamerasPair ** Dataset,int dataset_size);
void printNegativePairs(List * diffPairs,int num_of_cameras);
Dataset * train_test_split_pairs(CamerasPair ** pairsArray,int * Labels,int datasetSize,int stratify);
void createVectors(CamSpec ** camArray,int num_of_cameras);
void printVector(CamSpec ** camArray,int num_of_cameras);
CamerasPair ** create_RetrainDataset(CamSpec ** camArray, int num_of_cameras, int* num_of_retrain_specs);

retraining_set ** LR_retrain(retraining_set** retrainingArray,LogisticRegression* model,CamSpec ** camArray, int num_of_cameras, float threshold,int* num_of_retrain_specs,size_t vectorSize);
int compareRetrainingSet(const void * a, const void * b);
Xy_Split * resolve_transitivity_issues(Xy_Split * Xy_train,Clique*** cliqueIndex,int * numOfCliques,retraining_set** retrainingArray, int num_of_retrain_specs,int num_of_cameras, float threshold);
void destroyRetrainArray(retraining_set** retrainingArray, int size);

pairsArgs * new_pairsArgs(	LogisticRegression* model,CamSpec ** camArray,int num_of_cameras,float threshold,size_t VectorSize,int i);
void create_retrainScheduler(int numThreads);
void destroy_retrainScheduler();
void update_retrainArray(void * default_args);
