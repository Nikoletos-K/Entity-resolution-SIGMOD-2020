#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

#define BUFFER 100


void read_dir(char* nameOfDir){
	DIR * dir;
	struct dirent *info;
	char pathOfDir[1024];
	
	if((dir = opendir(nameOfDir))==NULL)
		return;


	while((info = readdir(dir)) != NULL){

		if(info->d_type == DT_DIR){
			if(!strcmp(info->d_name,".") || !strcmp(info->d_name,".."))
				continue;
			else{
				strcpy(pathOfDir,nameOfDir);
				strcat(pathOfDir,"/");
				strcat(pathOfDir,info->d_name);
				read_dir(pathOfDir);
			}
		}else{
			char path[512];
			char name[512];
			char* tok;
			char* dirname = basename(nameOfDir);
			strcpy(path,dirname);
			strcat(path,"//");
			strcpy(name,info->d_name);
			tok = strtok(name,".json");
			strcat(path,tok);
			printf("%s\n",path);
		}
	}

	closedir(dir);
	return;
}

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
	read_dir("2013_camera_specs");

	return 0;
}