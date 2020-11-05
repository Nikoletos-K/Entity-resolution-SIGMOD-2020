#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

#include "./../include/utils.h"

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

void read_dir(char* nameOfDir,HashTable * ht){
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
				read_dir(pathOfDir,ht);
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
			// printf("%s\n",path);
			CamSpec * cs = createCamSpec(path);
			HTInsert(ht,path,(void *) cs,stringComparator);
			// read_json(filename);
		}
	}

	closedir(dir);
	return;
}

HashTable * make_sets_from_csv(char * csvfile,HashTable * ht){

	FILE * csv = fopen(csvfile,"r");
	int line=0;

	// Reading csv line by line
	while(!feof(csv)){
		char buffer[BUFFER];
		fscanf(csv,"%[^\n]\n",buffer);
		// printf("%s\n",buffer );
		if(line!=0){
			char left_spec_id[BUFFER],right_spec_id[BUFFER];
			char * token = strtok(buffer,",");
			int spec_id=0,label;

			// Reading line
			while(token!=NULL){
				// printf("%s\n",token);
				switch(spec_id){
					case 0:
						strcpy(left_spec_id,token);
						printf("- %s\n",left_spec_id);fflush(stdout);
						break;
					case 1:
						strcpy(right_spec_id,token);
						printf("-- %s\n",right_spec_id);fflush(stdout);
						break;
					case 2:
						label = atoi(token);
						printf("--- %d\n",label);fflush(stdout);
						break;
				}
				spec_id++;
				token = strtok(NULL,",");
			}
			
			if(label == SAME_CAMERAS){
				CamSpec * left_node = HTSearch(ht,left_spec_id);
				CamSpec * right_node = HTSearch(ht,right_spec_id);

				if(left_node->set != right_node->set){
					left_node->set = mergeLists(left_node->set,right_node->set);
					right_node->set = left_node->set;
				}
			}
		}
		line++;
		printf("\n");fflush(stdout);
	}
	printf("%d\n",line );
	return ht;

}

int stringComparator(const void * str1,const void * str2){
    return strcmp((char*) str1,(char*) str2);
}
