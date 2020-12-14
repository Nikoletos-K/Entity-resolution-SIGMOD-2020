#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>

#include "./../../include/Dataset.h"
#include "./../../include/Vectorization.h"
#include "./../../include/Clique.h"
#include "./../../include/dictionary.h"
#include "./../../include/PairsImplementation.h"


// #include "./../../include/utils.h"

HashTable * Dictionary;
size_t DictionarySize;
size_t VectorSize = 5;  

dictNode ** DictionaryNodes;



int main(int argc,char ** argv){

	int i=1;
	int jsonDir,csvFile;

	/* Variables for counting time */
	double t1, t2, cpu_time,ft1,ft2,fcpu_time;
	struct tms tb1, tb2,ftb1,ftb2;
	double ticspersec;

	while(i<argc){
		if(!strcmp("-jd",argv[i]))
			jsonDir = i+1;
		else if(!strcmp("-csv",argv[i]))
			csvFile = i+1;	
		i+=2;
	}

	printf("\n--------------- ENTITY RESOLUTION SYSTEM ----------------\n");

	ft1 = (double) times(&ftb1);
	initializeDataStructures();
	HashTable * CameraHT = HTConstruct(HASHTABLE_SIZE);
	CamSpec ** camArray = malloc(sizeof(CamSpec *));
	int num_of_cameras=0;
	Clique** cliqueIndex;
	HashTable * stopwords = createStopWords("./../../data/stopwords.txt");
	Dictionary = HTConstruct(HASHTABLE_SIZE*5);
	DictionaryNodes = malloc(sizeof(dictNode*));


	/* ----------------   READING JSON DIRECTORY -------------------------- */
	ticspersec = (double) sysconf(_SC_CLK_TCK);
	t1 = (double) times(&tb1);
	printf("\n-> Starting reading directory (%s)  \n ",argv[jsonDir] );
	printf("   and informing data structures from given .json files\n ");

	camArray = read_dir(argv[jsonDir],CameraHT,camArray,&num_of_cameras,stopwords);

	printf("<- End of reading directory\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of reading directory and informing our data structures:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	
	/* ----------------   CREATING CLIQUES -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Starting reading .csv file  (%s)  \n ",argv[csvFile] );
	printf("   and informing sets\n ");


	DisJointSet * djSet = DJSConstruct(num_of_cameras,(void**)camArray);
	List * diffPairsList = createList();
	make_sets_from_csv(argv[csvFile],CameraHT,djSet,diffPairsList);
	int numOfCliques = 0;
	cliqueIndex = CreateCliques(djSet,&numOfCliques);

	printf("<- End\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of reading .csv file and forming cliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	/* ----------------   PRINTING CLIQUES -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Printing cliques: \n");

	// printPairs(cliqueIndex,numOfCliques); 
	
	printf(" <- End of printing cliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing cliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FORMING NEGATIVE CLIQUES -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming negative cliques: \n");
	
	cliqueIndex = createNegConnections(diffPairsList,cliqueIndex); 
	// List * differentCameras = createNegativePairs(cliqueIndex,numOfCliques);

	printf(" <- End of forming negative cliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of forming negative cliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FORMING Vectors -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming Vectors  \n");
	

	createVectors(camArray,num_of_cameras);
	// printVector(camArray,num_of_cameras);


	printf(" <- End of forming Vectors \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Vectors :\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FORMING DATASET -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming DATASET \n");
	
	// int dataset_size=0;
	// CamerasPair ** Dataset =  createDataset(sameCameras,differentCameras, &dataset_size);
	train_test_split(cliqueIndex,numOfCliques);
	// printDataset(Dataset,dataset_size);

	printf(" <- End of forming DATASET\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of DATASET:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);




	/* ----------------   TRAINING CLIQUES -------------------------- */


	t1 = (double) times(&tb1);
	printf("\n-> Training cliques  \n");
	
	float learning_rate = 0.1;
	float threshold = 0.00001;
	int max_epochs = 15;

	trainCliques(cliqueIndex,numOfCliques,learning_rate,threshold,max_epochs);


	printf(" <- End of Training cliques  \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Training cliques  :\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   TESTING CLIQUES -------------------------- */


	t1 = (double) times(&tb1);
	printf("\n-> Testing cliques  \n");
	
	
	float* accuracyArray =  testCliques(cliqueIndex,numOfCliques);
	float acc = 0.0;
	for (int i = 0; i < numOfCliques; i++){
		acc += accuracyArray[i];
		printf("Clique %3d has accuracy %6.2lf %% \n",i+1,accuracyArray[i] );
	}
	printf("\nAverage accuracy %5.2lf %% \n\n",acc/(float)numOfCliques);


	printf(" <- End of Testing cliques  \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Testing cliques  :\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   GRID SEARCH -------------------------- */

	// float learning_rates[3] =  {0.1,0.01,0.001};
	// int numofLr = 3;
	// float thresholds[4] = {0.001,0.0001,0.00001,0.000005};
	// int numofthreshold = 4;
	// int max_epochs[4] = {5,10,15,20};
	// int numOfmax_epochs = 4;

	// for(int i=0;i<numOfCliques;i++){

	// 	char fileName[30];
	// 	sprintf(fileName,"%d.GRIDSEARCH.txt",i+1);
	// 	FILE * GridSearchFile = fopen(fileName,"w+");

	// 	t1 = (double) times(&tb1);
	// 	printf("\n-> GRIDSEARCH %d \n",i+1);
		

	// 	HyperParameters * hp = constructHyperParameters(learning_rates,numofLr,thresholds,numofthreshold,max_epochs,numOfmax_epochs);
	// 	GridSearch(	cliqueIndex[i]->dataset->train,cliqueIndex[i]->dataset->test,hp,VectorSize,GridSearchFile);

	// 	fclose(GridSearchFile);
	// 	printf(" <- end  \n");
	// 	t2 = (double) times(&tb2);
	// 	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	// 	printf("PERFORMANCE of GRIDSEARCH - %d:\n",i+1);
	// 	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	// 	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);
	// }


	/* ----------------   FREE OF MEMORY -------------------------- */
	printf("\n\n-> Restoring memory\n");
	for(int i=0;i<num_of_cameras;i++)
		destroyCamSpec(camArray[i]);

	listNode * node = diffPairsList->firstNode;
	while(node != NULL){
		deletePair((CamerasPair*)node->data);
		node = node->nextNode;
	}

	deleteList(diffPairsList);

	for(int i=0;i<DictionarySize;i++)
		free(DictionaryNodes[i]);
	free(DictionaryNodes);

	free(accuracyArray);
	free(camArray);
	HTDestroy(CameraHT);
	HTDestroy(Dictionary);
	HTDestroy(stopwords);
	DJSDestructor(djSet);
	destroyCliques(cliqueIndex,numOfCliques);
	destroyDataStructures();
	printf("<- All frees done\n");

	ft2 = (double) times(&ftb2);
	fcpu_time = (double) ((ftb2.tms_utime + ftb2.tms_stime) - (ftb1.tms_utime + ftb1.tms_stime));
	printf("\nPERFORMANCE of system overall:\n");
	printf("- CPU_TIME: %.2lf sec\n",fcpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(ft2-ft1)/ticspersec);

	printf("\n----------------- END_OF_EXECUTION -------------------\n");
	return 0;
}