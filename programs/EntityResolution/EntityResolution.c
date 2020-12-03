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

	int i=1,print_stdout=1;
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
	HashTable * ht = HTConstruct(HASHTABLE_SIZE);
	CamSpec ** camArray = malloc(sizeof(CamSpec *));
	int num_of_cameras=0;
	Clique** cliqueIndex;

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
	make_sets_from_csv(argv[csvFile],ht,djSet);
	int numOfsets = 0;
	cliqueIndex = CreateSets(djSet,&numOfsets);

	printf("<- End\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of reading .csv file and forming cliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);

	t1 = (double) times(&tb1);
	printf("\n-> Printing cliques: \n");
	printPairs(cliqueIndex,numOfsets); 
	printf(" <- End of printing cliques\n");
	t2 = (double) times(&tb2);
	cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
	printf("PERFORMANCE of printing cliques:\n");
	printf("- CPU_TIME: %.2lf sec\n",cpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(t2-t1)/ticspersec);


	printf("\n-> Restoring memory\n");
	for(int i=0;i<num_of_cameras;i++)
		destroyCamSpec(camArray[i]);

	free(camArray);
	HTDestroy(ht);
	DJSDestructor(djSet);
	destroyDataStructures();
	printf("<- All frees done\n");

	ft2 = (double) times(&ftb2);
	fcpu_time = (double) ((ftb2.tms_utime + ftb2.tms_stime) - (ftb1.tms_utime + ftb1.tms_stime));
	printf("PERFORMANCE of system overall:\n");
	printf("- CPU_TIME: %.2lf sec\n",fcpu_time/ticspersec);
	printf("- REAL_TIME: %.2lf sec\n",(ft2-ft1)/ticspersec);

	printf("\n----------------- END_OF_EXECUTION -------------------\n");
	return 0;
}