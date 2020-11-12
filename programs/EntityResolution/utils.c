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

	FILE * json_file = fopen(filename,"r");		// open the json file
	
	char line[20000];

	while(!feof(json_file)){		// read every line until EOF is read
		
		fscanf(json_file,"%[^\n]\n",line);
		if(strcmp(line,"{") && strcmp(line,"}")){		

			char * key = strtok(line,":");		// split the line to key and value
			key++;		// remove the first "
			key[strlen(key)-1] = '\0';		 // remove the last " 
			cs = addJsonInfo(cs,key);			// create the json pair and \
														initialize it with the key
			
			char * value = strtok(NULL,"");	
			
			if(strcmp(value," [")){		// if value is not a list 
				value = value+2;

				if(value[strlen(value)-1] == ',')	// if this is not the last pair in the file
					value[strlen(value)-2] = '\0';
				else 
					value[strlen(value)-1] = '\0';
				cs = addValuetoCS(cs, value);			// add the value
				
			}else{
				fscanf(json_file,"%[^\n]\n",line);		// read the next line
				while(!feof(json_file) && strcmp(line,"],")){		// if it's not the EOF or the endo of list
					value = line;
					value++;		// remove the first "

					if(value[strlen(value)-1] == ',')	// if this is not the last pair in the list
						value[strlen(value)-2] = '\0';
					else 
						value[strlen(value)-1] = '\0';
					
					cs = addValuetoCS(cs, value);		// add every value of the list
					fscanf(json_file,"%[^\n]\n",line);		// read next line
				}
			}
			
		}
		int c = getc(json_file);
		if(c == '}'){		// in case there isn't '\n' after }
			break;
		}
		else
			ungetc(c,json_file);

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

	while((info = readdir(dir)) != NULL){		// read every element in the directory

		if(info->d_type == DT_DIR){		// if it is a directory do recursion
			if(!strcmp(info->d_name,".") || !strcmp(info->d_name,".."))
				continue;
			else{
				strcpy(pathOfDir,nameOfDir);
				strcat(pathOfDir,"/");
				strcat(pathOfDir,info->d_name);
				camArray = read_dir(pathOfDir,ht,camArray,array_position);
			}
		}else{				// if it is a file
			char path[512];			// create the name of the spec
			char name[512];			// save the name of the file 
			char filename[512];			// save the full path of the file
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
			CamSpec * cs = createCamSpec(path,*array_position);		// create the struct for the json
			HTInsert(ht,path,(void *) cs,stringComparator);			// insert it to the hashtable
			camArray = realloc(camArray,(*array_position+1)*sizeof(CamSpec*));
			camArray[*array_position] = cs;		// add it to the array for the disjointtest
			(*array_position)++;
			cs = read_jsonSpecs(filename,cs);		// read the file and save the json info
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
			char * token = strtok(buffer,",");		// split the line
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
			
			if(label == SAME_CAMERAS){		// if label is 1
				CamSpec * left_node = HTSearch(ht,left_spec_id,stringComparator);		// search left in the hashtable
				CamSpec * right_node = HTSearch(ht,right_spec_id,stringComparator);		// search right in the hashtable
				if(left_node != NULL && right_node != NULL)
					DJSUnion(djSet,left_node->arrayPosition,right_node->arrayPosition);		// add them as same in the disjoint set			
			}
		}
		line++;
	}
	fclose(csv);
	return ht;

}

void printPairs(DisJointSet * djSet,int print_stdout){

	FILE * output;
	if(print_stdout)	// if user wants data to be printed in the stdout
		output = stdout;
	else
		output = fopen("PAIRS.txt","w+");		// or in file


	int parent;
	CamSpec** camArray = (CamSpec**) (djSet->objectArray);

	for(int i=0;i<djSet->size;i++){		// find root parent of every spec
		parent = DJSFindParent(djSet,i);
		if(parent!=i)
			insert_toList(camArray[parent]->set,camArray[i]);	// insert it to parent's list
	}

	for(int i=0;i<djSet->size;i++){	// for every spec
		if(!oneNodeList(camArray[i]->set))		// if the list was not empty
			printForward(camArray[i]->set,output,printCameraName);	// print every pair in the list
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

