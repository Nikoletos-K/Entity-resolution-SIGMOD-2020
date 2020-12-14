#pragma once

#include "./Clique.h"

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
List * createNegativePairs(Clique ** CliqueIndex,int numOfcliques);

CamerasPair ** create_PairsDataset(List * sameCameras,List * differentCameras,int * dataset_size);
void printDataset(CamerasPair ** Dataset,int dataset_size);
