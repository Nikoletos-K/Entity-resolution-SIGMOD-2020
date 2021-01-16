#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <math.h>

#include "./../../include/Dataset.h"
#include "./../../include/Vectorization.h"
#include "./../../include/Clique.h"
#include "./../../include/dictionary.h"
#include "./../../include/PairsImplementation.h"

HashTable * Dictionary;
size_t DictionarySize;
size_t VectorSize = 1000;  

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
	HTDestroy(Dictionary);

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
	printf("\n-> Printing same cameras in PAIRS.csv: \n");

	List * sameCameras = printPairs(cliqueIndex,numOfCliques); 
	
	printf(" <- End of printing same cameras\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing same cameras:\n");
	printf("- CPU_TIME:  %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FORMING NEGATIVE CLIQUES -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming negative cliques: \n");
	FILE * output = fopen("NEGATIVE_PAIRS.csv","w+");
	fprintf(output, "left_spec_id, right_spec_id,\n");
	cliqueIndex = createNegConnections(diffPairsList,cliqueIndex); 
	List * differentCameras = createNegativePairs(cliqueIndex,numOfCliques,output);
	fclose(output);
	printf(" <- End of forming negative cliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of forming negative cliques:\n");
	printf("- CPU_TIME:  %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	/* ----------------   FORMING Vectors -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming Vectors  \n");
	

	createVectors(camArray,num_of_cameras);

	printf(" <- End of forming Vectors \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Vectors :\n");
	printf("- CPU_TIME:  %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FORMING DATASET -------------------------- */
	t1 = (double) times(&tb1);
	printf("\n-> Forming DATASET \n");
	
	int same = get_listSize(sameCameras),different = get_listSize(differentCameras);
	printf("Same Cameras:      %d\n", same);
	printf("Different Cameras: %d\n", different);
	int dataset_size = same+different;
	printf("Dataset size:      %d\n",dataset_size);
	int stratify = (int) floor((float)dataset_size/(float)same);
	printf("Stratified factor: %d\n",stratify );
	int * Labels = malloc(sizeof(int)*(dataset_size));

	CamerasPair ** pairDataset =  create_PairsDataset(sameCameras,differentCameras,Labels, dataset_size,stratify);

	printf(" <- End of forming DATASET\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of DATASET:\n");
	printf("- CPU_TIME:  %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	HTDestroy(stopwords);

	/* ----------------   PRINTING DATASET -------------------------- */
	
	t1 = (double) times(&tb1);
	printf("\n-> Printing dataset to DATASET.csv \n");
	
	printDataset(pairDataset,dataset_size);

	printf(" <- End of printing dataset\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing dataset:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	
	/* ----------------   TRAIN_TEST_VALIDATION -------------------------- */
	
	t1 = (double) times(&tb1);
	printf("\n-> Forming train,test and validation sets \n");
	
	Dataset * vectorizedDataset =  train_test_split_pairs(pairDataset,Labels,dataset_size,stratify+1);
	printf("Train      size: %ld\n",vectorizedDataset->train->size );
	printf("Test       size: %ld\n",vectorizedDataset->test->size );
	printf("Validation size: %ld\n",vectorizedDataset->validation->size );



	printf(" <- End of forming train,test and validation sets\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of forming train,test and validation sets:\n");
	printf("- CPU_TIME:  %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	listNode * node = diffPairsList->firstNode;
	while(node != NULL){
		deletePair((CamerasPair*)node->data);
		node = node->nextNode;
	}

	deleteList(diffPairsList);

	DJSDestructor(djSet);
	destroyCliques(cliqueIndex,numOfCliques);

	/* ----------------   TRAINING CLIQUES -------------------------- */


	t1 = (double) times(&tb1);
	printf("\n-> Creating - Training model  \n");
	float learning_rate,threshold;
	int epochs;

	if(!strcmp("./../../data/sigmod_medium_labelled_dataset.csv",argv[csvFile])){
		learning_rate = 0.001;
		threshold = 1;
		epochs = 20;

	}else{
		learning_rate = 0.001;
		threshold = 0.1;
		epochs = 50;

	}
	printf("Learning rate: %lf\n", learning_rate);
	printf("Threshold:     %lf\n", threshold);
	printf("Max epochs:    %d\n", epochs);

	LogisticRegression* LR_Model = LR_construct(VectorSize*2,learning_rate,threshold,epochs);
	LR_fit(LR_Model,vectorizedDataset->train);

	printf(" <- End of Creating - Training model \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Creating - Training model  :\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	// while(threshold<0.5){

	// 	LR_fit(LR_Model,vectorizedDataset->train);
	// }

	printf(" <- End of Creating - Training model \n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of Creating - Training model  :\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   TESTING CLIQUES -------------------------- */


	// t1 = (double) times(&tb1);
	// printf("\n-> Testing model  \n");
	
	// LR_Evaluation(LR_Model,vectorizedDataset->test,stdout);

	// printf(" <- End of Testing model  \n");
	// t2 = (double) times(&tb2);
	// cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	// printf("PERFORMANCE of Testing model  :\n");
	// printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	// printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   GRID SEARCH -------------------------- */
	// t1 = (double) times(&tb1);
	// printf("\n-> GRIDSEARCH-%s \n",argv[csvFile]);

	// float learning_rates[4] =  {1,0.1,0.01,0.001};
	// int numofLr = 4;
	// float thresholds[5] = {1, 0.1 ,0.001,0.0001,0.00001};
	// int numofthreshold = 5;
	// int max_epochs[5] = {5,10,20,50,100};
	// int numOfmax_epochs = 5;

	// FILE* fp = fopen("PAIRS_GRIDSEARCH-Medium.txt","w");


	// HyperParameters * hp = constructHyperParameters(learning_rates,numofLr,thresholds,numofthreshold,max_epochs,numOfmax_epochs);
	// GridSearch(	vectorizedDataset->train,vectorizedDataset->test,hp,VectorSize,fp);

	// printf(" <- end  \n");
	// t2 = (double) times(&tb2);
	// cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	// printf("PERFORMANCE of GRIDSEARCH \n");
	// printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	// printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	/* ----------------   FREE OF MEMORY -------------------------- */


	printf("\n\n-> Restoring memory\n");
	
	for(int i=0;i<dataset_size;i++)
		deletePair(pairDataset[i]);
	free(pairDataset);

	free(Labels);
	LR_destroy(LR_Model);



	deleteList(sameCameras);
	deleteList(differentCameras);	
	
	for(int i=0;i<DictionarySize;i++)
		free(DictionaryNodes[i]);
	free(DictionaryNodes);


	for(int i=0;i<num_of_cameras;i++)
		destroyCamSpec(camArray[i]);
	free(camArray);

	HTDestroy(CameraHT);

	destroy_Dataset(vectorizedDataset,1);	

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