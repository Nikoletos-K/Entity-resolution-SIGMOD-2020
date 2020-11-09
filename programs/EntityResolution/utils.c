#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>

#include "./../../include/utils.h"

CamSpec * read_jsonSpecs(char* filename,CamSpec * cs){

	FILE * json_file = fopen(filename,"r");

	while(!feof(json_file)){
		char line[1024];
		fscanf(json_file,"%[^\n]\n",line);
		if(strcmp(line,"{") && strcmp(line,"}")){

			printf("\nLINE %s\n",line );
			char * key = strtok(line,":");
			key++;
			key[strlen(key)-1] = '\0';

			char * value = strtok(NULL,"");	
			printf("%s\n",filename );
			if(!strcmp(value,"[")){
				value = value+2;
				value[strlen(value)-2] = '\0';
			}
			cs = addJsonInfo(cs,key,value);
			printf("KEY:   %s \nVALUE:   %s\n\n",key,value);

		}
	}
	fclose(json_file);

	return cs;
}

CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position){
	DIR * dir;
	struct dirent *info;
	char pathOfDir[BUFFER];
	
	if((dir = opendir(nameOfDir))==NULL)
		return camArray;

	while((info = readdir(dir)) != NULL){

		if(info->d_type == DT_DIR){
			if(!strcmp(info->d_name,".") || !strcmp(info->d_name,".."))
				continue;
			else{
				strcpy(pathOfDir,nameOfDir);
				strcat(pathOfDir,"/");
				strcat(pathOfDir,info->d_name);
				camArray = read_dir(pathOfDir,ht,camArray,array_position);
			}
		}else{
			char path[512];
			char name[512];
			char filename[512];
			strcpy(filename,nameOfDir);
			strcat(filename,"/");
			strcat(filename,info->d_name);
			char* tok;
			char* dirname = basename(nameOfDir);
			strcpy(path,dirname);
			strcat(path,"//");
			strcpy(name,info->d_name);
			tok = strtok(name,".json");
			strcat(path,tok);
			CamSpec * cs = createCamSpec(path,*array_position);
			HTInsert(ht,path,(void *) cs,stringComparator);
			camArray = realloc(camArray,(*array_position+1)*sizeof(CamSpec*));
			camArray[*array_position] = cs;
			(*array_position)++;
			cs = read_jsonSpecs(filename,cs);
		}
	}

	closedir(dir);
	return camArray;
}

HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet *djSet){

	FILE * csv = fopen(csvfile,"r");
	int line=0;

	// Reading csv line by line
	while(!feof(csv)){
		char buffer[BUFFER];
		fscanf(csv,"%[^\n]\n",buffer);

		if(line!=0){
			char left_spec_id[BUFFER],right_spec_id[BUFFER];
			char * token = strtok(buffer,",");
			int spec_id=0,label;

			// Reading line
			while(token!=NULL){

				switch(spec_id){
					case 0:
						strcpy(left_spec_id,token);
						break;
					case 1:
						strcpy(right_spec_id,token);
						break;
					case 2:
						label = atoi(token);
						break;
				}
				spec_id++;
				token = strtok(NULL,",");
			}
			
			if(label == SAME_CAMERAS){
				CamSpec * left_node = HTSearch(ht,left_spec_id,stringComparator);
				CamSpec * right_node = HTSearch(ht,right_spec_id,stringComparator);
				DJSUnion(djSet,left_node->arrayPosition,right_node->arrayPosition);				
			}
		}
		line++;
	}
	fclose(csv);
	return ht;

}

void printPairs(DisJointSet * djSet,int print_stdout){

	FILE * output;
	if(print_stdout)
		output = stdout;
	else
		output = fopen("PAIRS.txt","w+");


	int parent;
	CamSpec** camArray = (CamSpec**) (djSet->objectArray);

	for(int i=0;i<djSet->size;i++){
		parent = DJSFindParent(djSet,i);
		if(parent!=i)
			insert_toList(camArray[parent]->set,camArray[i]);
	}

	for(int i=0;i<djSet->size;i++){
		if(!oneNodeList(camArray[i]->set))
			printForward(camArray[i]->set,output,printCameraName);
		fflush(stdout);
	}

	if(!print_stdout)
		fclose(output);
}

int stringComparator(const void * str1,const void * str2){
    return strcmp((char*) str1,(char*) str2);
}

void printCameraName(void * data,FILE * output){
	fprintf(output,"%s",((CamSpec*)data)->name);
}

