#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <math.h>


#include "./../../include/utils.h"


CamSpec * read_jsonSpecs(char* filename,CamSpec * cs){

	FILE * json_file = fopen(filename,"r");
	
	// printf("%s\n",filename );
	char line[20000];

	while(!feof(json_file)){
		
		fscanf(json_file,"%[^\n]\n",line);
		if(strcmp(line,"{") && strcmp(line,"}")){

			// printf("\nLINE %s\n",line );
			char * key = strtok(line,":");
			key++;
			key[strlen(key)-1] = '\0';
			// printf("KEY: %s\n",key );
			cs = addJsonInfo(cs,key);
			
			char * value = strtok(NULL,"");	
			
			if(strcmp(value," [")){
				value = value+2;

				if(value[strlen(value)-1] == ',')
					value[strlen(value)-2] = '\0';
				else 
					value[strlen(value)-1] = '\0';
				cs = addValuetoCS(cs, value);
				// printf("1.VALUE:   %s\n\n",value);

			}else{
				fscanf(json_file,"%[^\n]\n",line);
				while(!feof(json_file) && strcmp(line,"],")){
					value = line;
					value++;

					if(value[strlen(value)-1] == ',')
						value[strlen(value)-2] = '\0';
					else 
						value[strlen(value)-1] = '\0';
					
					cs = addValuetoCS(cs, value);
					// printf("2.VALUE:   %s\n\n",value);
					fscanf(json_file,"%[^\n]\n",line);
				}
			}
			
		}
		int c = getc(json_file);
		if(c == '}'){
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

					DiffCamerasPair * pair = createPair(left_node,right_node);
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

void printPairs(Qlique** qliquesArray,int numOfsets ){

	FILE * output;

	output = fopen("PAIRS.txt","w+");		// or in file


	for(int i=0;i<numOfsets;i++)	// for every spec
		printForward(qliquesArray[i]->set,output,printCameraName);	// print every pair in the list
	

	fclose(output);

}

Qlique** CreateSets(DisJointSet * djSet,int* numOfsets){
	int parent;
	Qlique** qliquesArray = malloc(sizeof(Qlique*));
	List* set;
	CamSpec* data;
	
	CamSpec** camArray = (CamSpec**) (djSet->objectArray);

	for(int i=0;i<djSet->size;i++){
		parent = DJSFindParent(djSet,i);
		if(parent!=i)
			insert_toList(camArray[parent]->set,camArray[i]);
	}

	for(int i=0;i<djSet->size;i++){	// for every spec
		if(!oneNodeList(camArray[i]->set)){		
			qliquesArray = realloc(qliquesArray,(*numOfsets+1)*sizeof(Qlique*));
			qliquesArray[*numOfsets] = malloc(sizeof(Qlique));
			qliquesArray[*numOfsets]->set = camArray[i]->set;
			qliquesArray[*numOfsets]->numOfNegativeQliques = 0;
			qliquesArray[*numOfsets]->negativeQliques = NULL;

			(*numOfsets)++;

			set = (qliquesArray[*numOfsets-1])->set;

			listNode * node = set->firstNode;
	
			while(node!=NULL){

				data = (CamSpec*) node->data;
				data->arrayPosition = *numOfsets-1;
				node = node->nextNode;
			}
		}
			
	}
	return qliquesArray;
}

Qlique** createNegConnections(List * diffPairsList,Qlique ** qliqueIndex){

	BF * bloomFilter = createBF((unsigned int) get_listSize(diffPairsList));

	listNode * node = diffPairsList->firstNode;
	while(node != NULL){

		DiffCamerasPair * pair = (DiffCamerasPair*) node->data;

		int qliqueKey_1 = pair->camera1->arrayPosition;
		int qliqueKey_2 = pair->camera2->arrayPosition;

		int encodedKey = CantorEncode(qliqueKey_1,qliqueKey_2);

		if(!checkBF(bloomFilter,(void*) &encodedKey)){
			// insert_toList(newList,(void*)pair);
			insertBF(bloomFilter,(void*) &encodedKey);
			insert_NegConnection(qliqueIndex[qliqueKey_1],qliqueKey_1);
			insert_NegConnection(qliqueIndex[qliqueKey_2],qliqueKey_2);

		}

		node = node->nextNode;
	}

	return qliqueIndex;
}

Qlique * insert_NegConnection(Qlique * ql,int arrayPosition){

	if(ql->negativeQliques == NULL)
		ql->negativeQliques = malloc(sizeof(int));
	
	else
		ql->negativeQliques = realloc(ql->negativeQliques,(ql->numOfNegativeQliques+1)*sizeof(int));
	

	ql->negativeQliques[ql->numOfNegativeQliques] = arrayPosition;
	(ql->numOfNegativeQliques)++;

	return ql;
}

int stringComparator(const void * str1,const void * str2){
    return strcmp((char*) str1,(char*) str2);
}

void printCameraName(void * data,FILE * output){
	fprintf(output,"%s",((CamSpec*)data)->name);
}

int CantorEncode(int num1,int num2){
	return ((num1+num2)*(num1+num2+1)/2)+num2;
}

int CantorDecode(int cantor_number,int* num1,int* num2){

	int w = floor(((sqrt(8*cantor_number+1))-1)/2);
	int t = (w*(w + 1))/2;
	*num2 = cantor_number-t;
	*num1 = w - *num2; 
	return cantor_number;
}
void destroySets(Qlique** qliquesArray,int numOfsets){
	free(qliquesArray);
}

DiffCamerasPair * createPair(CamSpec * c1, CamSpec * c2){

	DiffCamerasPair * pair = malloc(sizeof(DiffCamerasPair));
	pair-> camera1 = c1;
	pair-> camera2 = c2;

	return pair;

}

void deletePair(DiffCamerasPair * pair){
	free(pair);
}