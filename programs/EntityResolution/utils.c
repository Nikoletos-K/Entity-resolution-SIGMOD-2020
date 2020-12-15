#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "./../../include/dictionary.h"
#include "./../../include/PairsImplementation.h"

CamSpec * read_jsonSpecs(char* filename,CamSpec * cs,HashTable* stopwords){

	FILE * json_file = fopen(filename,"r");		// open the json file
	
	char line[20000];

	const char s[2] = " ";
   	char *token;

	while(!feof(json_file)){		// read every line until EOF is read
		
		fscanf(json_file,"%[^\n]\n",line);

		if(strcmp(line,"{") && strcmp(line,"}")){	

			char * key = strtok(line,":");		// split the line to key and value
			key++;		// remove the first "
			key[strlen(key)-1] = '\0';		 // remove the last " 
			cs = addJsonInfo(cs,key);			// create the json pair and initialize it with the key  

			char * value = strtok(NULL,"");	

			// token = strtok(key, s);

			// if(token==NULL)
			// 	addWord(key,cs,stopwords);

			// while( token != NULL ) {
			//   addWord(token,cs,stopwords);		    
		 //      token = strtok(NULL, s);
		 //    }
			
			if(strcmp(value," [")){		// if value is not a list 
				value = value+2;

				if(value[strlen(value)-1] == ',')	// if this is not the last pair in the file
					value[strlen(value)-2] = '\0';
				else 
					value[strlen(value)-1] = '\0';
				cs = addValuetoCS(cs, value);			// add the value

				token = strtok(value, s);

				if(token==NULL)
					addWord(value,cs,stopwords);

				while( token != NULL ) {
				  addWord(token,cs,stopwords);		    
			      token = strtok(NULL, s);
			   }
				
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

					token = strtok(value, s);

					if(token==NULL)
						addWord(value,cs,stopwords);

					while( token != NULL ) {
					  addWord(token,cs,stopwords);		    
				      token = strtok(NULL, s);
				    }

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


CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position,HashTable* stopwords){
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
				camArray = read_dir(pathOfDir,ht,camArray,array_position,stopwords);
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
			cs = read_jsonSpecs(filename,cs,stopwords);		// read the file and save the json info
		}
	}

	closedir(dir);
	return camArray;
}

HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet *djSet,List * diffPairsList){

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
	
			CamSpec * left_node = HTSearch(ht,left_spec_id,stringComparator);
			CamSpec * right_node = HTSearch(ht,right_spec_id,stringComparator);

			if(left_node != NULL && right_node!=NULL){	
					
				if(label == SAME_CAMERAS)
				
					DJSUnion(djSet,left_node->arrayPosition,right_node->arrayPosition);				
				
				else if(label == DIFFERENT_CAMERAS){

					CamerasPair * pair = createPair(left_node,right_node);
					insert_toList(diffPairsList,(void*) pair);

				}else
					fprintf(stderr,"make_sets_from_csv: Only 1 and 0 accepted\n");
			}
		}
		line++;
	}
	fclose(csv);
	return ht;

}


/*-------------------------------- Clique functions -------------------------------*/


int stringComparator(const void * str1,const void * str2){
    return strcmp((char*) str1,(char*) str2);
}

void printCameraName(void * data,FILE * output){
	fprintf(output,"%s",((CamSpec*)data)->name);
}
