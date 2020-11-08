#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>

#include "./../../include/utils.h"

void read_json(char* filename){
	// FILE * fp;
	// char word[1024];
	
	// printf("-----------------------------------------\n" );
	// fp = fopen(filename,"r");
	// CamSpec* js = malloc(sizeof(CamSpec));
	// createJson(js);
	// char temp[2] = "\"";
	
	// while(fscanf(fp,"%[^:]s",word)==1){
	// 	// printf("1. %s\n",word );
	// 	// fscanf(fp,"%[^\"]s",word);
	// 	// printf("2. %s\n",word );
	// 	// fscanf(fp,"%s",word);
	// 	// printf("3. %s\n",word );
	// 	// fscanf(fp,"%[^\"]s",word);
	// 	// printf("4. %s\n",word );
	// 	// fscanf(fp,"%[^\"]s",word);
	// }
	// fclose(fp);
}

CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position){
	DIR * dir;
	struct dirent *info;
	char pathOfDir[1024];
	
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


			// read_json(filename);
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

