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
	read_dir(argv[jsonDir],ht);
	printf("%d\n",csvFile );
	// HTSearch()

	// for(int i=0;i<HASHTABLE_SIZE;i++){
			
	// 	if(ht->Table[i]->rbtRoot!=NULL)
	// 		RBTPrintTree(ht->Table[i]->rbtRoot,printCamSpec);
	// }

	make_sets_from_csv(argv[csvFile],ht);
	return 0;
}