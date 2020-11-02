#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 100



int read_csv(char * filename){

	FILE * csv = fopen(filename,"r");
	int line=0;
	while(!feof(csv)){
		char buffer[BUFFER];
		fscanf(csv,"%s",buffer);
		printf("%s\n",buffer);
		if(line!=0){
			char * token = strtok(buffer,",");
			while(token!=NULL){
				printf("%s\n",token );
				token = strtok(NULL,",");
			}
		}
		line++;
	}
	printf("%d\n",line );
	return 1;

}

int main(){

	read_csv("sigmod_medium_labelled_dataset.csv");

}