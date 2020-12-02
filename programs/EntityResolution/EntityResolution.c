#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>

#include "./../../include/utils.h"


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
	// while(1){
	// 	printf("Printing results to stdout? [y/n]: ");
	// 	char answer[BUFFER];
	// 	scanf("%s",answer);
	// 	if(!strcmp(answer,"y")){
	// 	 	print_stdout = 1;
	// 	 	printf("Identical cameras pairs will be printed in stdout\n");
	// 	 	break;
	// 	}else if(!strcmp(answer,"n")){
	// 		print_stdout = 0;
	// 	 	printf("Identical cameras pairs will be printed in file named: PAIRS.txt in the current directory\n");
	// 		break;
	// 	}else
	// 		printf("Press 'y' for printing in stdout or 'n' for printing in a file \n");
	// }
	ft1 = (double) times(&ftb1);
	initializeDataStructures();
	HashTable * ht = HTConstruct(HASHTABLE_SIZE);
	CamSpec ** camArray = malloc(sizeof(CamSpec *));
	int num_of_cameras=0;
	Qlique** qliqueIndex;

	ticspersec = (double) sysconf(_SC_CLK_TCK);
	t1 = (double) times(&tb1);
	printf("\n-> Starting reading directory (%s)  \n ",argv[jsonDir] );
	printf("   and informing data structures from given .json files\n ");

	camArray = read_dir(argv[jsonDir],ht,camArray,&num_of_cameras);

	printf("<- End of reading directory\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of reading directory and informing our data structures:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	t1 = (double) times(&tb1);
	printf("\n-> Starting reading .csv file  (%s)  \n ",argv[csvFile] );
	printf("   and informing sets\n ");

	DisJointSet * djSet = DJSConstruct(num_of_cameras,(void**)camArray);

	List * diffPairsList = createList();
	make_sets_from_csv(argv[csvFile],ht,djSet,diffPairsList);
	int numOfsets = 0;
	qliqueIndex = CreateSets(djSet,&numOfsets);

	printf("<- End\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of reading .csv file and forming qliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	t1 = (double) times(&tb1);
	printf("\n-> Printing qliques: \n");
	printPairs(qliqueIndex,numOfsets); 
	printf(" <- End of printing qliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing qliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	t1 = (double) times(&tb1);
	printf("\n-> Forming negative qliques: \n");
	qliqueIndex = createNegConnections(diffPairsList,qliqueIndex); 
	printf(" <- End of forming negative qliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing qliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	printf("-> Restoring memory\n");
	for(int i=0;i<num_of_cameras;i++)
		destroyCamSpec(camArray[i]);

	free(camArray);
	HTDestroy(ht);
	DJSDestructor(djSet);
	destroyDataStructures();
	destroySets(qliqueIndex,numOfsets);
	printf("<- All frees done\n");

	ft2 = (double) times(&ftb2);
	fcpu_time = (double) ((ftb2.tms_utime + ftb2.tms_stime) - (ftb1.tms_utime + ftb1.tms_stime));
	printf("\nPERFORMANCE of system overall:\n");
	printf("- CPU_TIME: %.2lf sec\n",fcpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(ft2-ft1)/ticspersec);

	printf("\n----------------- END_OF_EXECUTION -------------------\n");
	return 0;
}