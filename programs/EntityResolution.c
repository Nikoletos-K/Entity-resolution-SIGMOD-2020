#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

#include "./../include/utils.h"
// #include "./../include/utils.h"


int main(int argc,char ** argv){

	int i=1;
	int jsonDir,csvFile;
	while(i<argc){
		if(!strcmp("-jd",argv[i]))
			jsonDir = i+1;
		else if(!strcmp("-csv",argv[i]))
			csvFile = i+1;	
		i+=2;
	}

	initializeDataStructures();
	
	HashTable * ht = HTConstruct(HASHTABLE_SIZE);

	CamSpec ** camArray = malloc(sizeof(CamSpec *));
	int num_of_cameras=0;
	camArray = read_dir(argv[jsonDir],ht,camArray,&num_of_cameras);

	DisJointSet * djSet = DSJConstruct(num_of_cameras,(void**)camArray);
	make_sets_from_csv(argv[csvFile],ht,djSet);

	return 0;
}