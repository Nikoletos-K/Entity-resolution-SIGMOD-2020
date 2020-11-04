#include "./../include/utils.h"

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
	read_dir(argv[jsonDir],ht);
	printf("%d\n",csvFile );

	return 0;
}