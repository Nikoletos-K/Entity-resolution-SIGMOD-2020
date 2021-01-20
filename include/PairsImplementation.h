#pragma once

#include "./Clique.h"
#include "./Vectorization.h"



typedef struct CamerasPair{
	
	CamSpec * camera1;
	CamSpec * camera2;
	int trueLabel;

} CamerasPair;


List * printPairs(Clique** setsList,int numOfsets );
void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list);

CamerasPair * createPair(CamSpec * c1, CamSpec * c2);
void deletePair(CamerasPair * pair);
void setLabel(CamerasPair *  pair,int label);
Clique** createNegConnections(List * diffPairsList,Clique ** CliqueIndex);
Clique * insert_uniqueNegConnection(Clique * cl,int arrayPosition);
Clique * insert_NegConnection(Clique * ql,int arrayPosition);
List * createNegativePairs(Clique ** CliqueIndex,int numOfcliques,FILE * file);

CamerasPair ** create_PairsDataset(List * sameCameras,List * differentCameras,int * Labels,int dataset_size,int stratify);
void printDataset(CamerasPair ** Dataset,int dataset_size);
void printNegativePairs(List * diffPairs);
Dataset * train_test_split_pairs(CamerasPair ** pairsArray,int * Labels,int datasetSize,int stratify);
void createVectors(CamSpec ** camArray,int num_of_cameras);
void printVector(CamSpec ** camArray,int num_of_cameras);