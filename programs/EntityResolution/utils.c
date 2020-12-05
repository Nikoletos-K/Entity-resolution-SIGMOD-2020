#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>

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
			cs = addJsonInfo(cs,key);			// create the json pair and initialize it with the key
			
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

List * printPairs(Clique** cliquesArray,int numOfsets ){

	FILE * output;
	List * sameCameras_list = createList();

	output = fopen("PAIRS.csv","w+");		// or in file
	fprintf(output, "left_spec_id, right_spec_id, label\n");
	for(int i=0;i<numOfsets;i++)	// for every spec
		printForward(cliquesArray[i]->set,output,printCameraName,sameCameras_list);	// print every pair in the list
	
	fclose(output);

	return sameCameras_list;
}

void printForward(List * list,FILE * output,void (*printData)(void*,FILE *),List * sameCameras_list){

	listNode * leftNode = list->firstNode,* rightNode;
	
	while(leftNode!=NULL){

		rightNode = leftNode->nextNode;
		while(rightNode!=NULL){

			printData(leftNode->data,output);
			fprintf(output,", ");
			printData(rightNode->data,output);
			fprintf(output,"\n");

			CamerasPair * pair = createPair((CamSpec *) leftNode->data, (CamSpec *) rightNode->data);
			insert_toList(sameCameras_list,(void*) pair);
			rightNode = rightNode->nextNode;
		}
		leftNode = leftNode->nextNode;
	}
}
/*-------------------------------- Clique functions -------------------------------*/


Clique** CreateSets(DisJointSet * djSet,int* numOfsets){
	int parent;
	Clique** cliquesArray = malloc(sizeof(Clique*));
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
			cliquesArray = realloc(cliquesArray,(*numOfsets+1)*sizeof(Clique*));
			cliquesArray[*numOfsets] = malloc(sizeof(Clique));
			cliquesArray[*numOfsets]->set = camArray[i]->set;
			cliquesArray[*numOfsets]->numOfNegativeCliques = 0;
			cliquesArray[*numOfsets]->negativeCliques = NULL;
			cliquesArray[*numOfsets]->numOfUnique_negativeCliques = 0;
			cliquesArray[*numOfsets]->unique_negativeCliques = NULL;			

			(*numOfsets)++;

			set = (cliquesArray[*numOfsets-1])->set;

			listNode * node = set->firstNode;
	
			while(node!=NULL){

				data = (CamSpec*) node->data;
				data->arrayPosition = *numOfsets-1;
				node = node->nextNode;
			}
		}	
	}

	for (int i = 0; i < *numOfsets; i++)
		cliquesArray[i]->bitArray =  createBF(*numOfsets);
	
	return cliquesArray;
}

void destroySets(Clique** cliquesArray,int numOfsets){
	free(cliquesArray);
}

Clique** createNegConnections(List * diffPairsList,Clique ** cliqueIndex){

	listNode * node = diffPairsList->firstNode;
	while(node != NULL){

		CamerasPair * pair = (CamerasPair*) node->data;

		int cliqueKey_1 = pair->camera1->arrayPosition;
		int cliqueKey_2 = pair->camera2->arrayPosition;

		if(!checkBit(cliqueIndex[cliqueKey_1]->bitArray,cliqueKey_2)){
			setBit(cliqueIndex[cliqueKey_1]->bitArray,cliqueKey_2);
			cliqueIndex[cliqueKey_1] = insert_NegConnection(cliqueIndex[cliqueKey_1],cliqueKey_2);
			cliqueIndex[cliqueKey_1] = insert_uniqueNegConnection(cliqueIndex[cliqueKey_1],cliqueKey_2);
		}

		if(!checkBit(cliqueIndex[cliqueKey_2]->bitArray,cliqueKey_1)){
			setBit(cliqueIndex[cliqueKey_2]->bitArray,cliqueKey_1);
			cliqueIndex[cliqueKey_2] = insert_NegConnection(cliqueIndex[cliqueKey_2],cliqueKey_1);	
		}

		node = node->nextNode;
	}

	return cliqueIndex;
}


Clique * insert_NegConnection(Clique * cl,int arrayPosition){

	if(cl->negativeCliques == NULL)
		cl->negativeCliques = malloc(sizeof(int));
	
	else
		cl->negativeCliques = realloc(cl->negativeCliques,(cl->numOfNegativeCliques+1)*sizeof(int));
	

	cl->negativeCliques[cl->numOfNegativeCliques] = arrayPosition;
	(cl->numOfNegativeCliques)++;

	return cl;
}

Clique * insert_uniqueNegConnection(Clique * cl,int arrayPosition){

	if(cl->unique_negativeCliques == NULL)
		cl->unique_negativeCliques = malloc(sizeof(int));
	
	else
		cl->unique_negativeCliques = realloc(cl->unique_negativeCliques,(cl->numOfUnique_negativeCliques+1)*sizeof(int));
	

	cl->unique_negativeCliques[cl->numOfUnique_negativeCliques] = arrayPosition;
	(cl->numOfUnique_negativeCliques)++;

	return cl;
}

CamerasPair * createPair(CamSpec * c1, CamSpec * c2){

	CamerasPair * pair = malloc(sizeof(CamerasPair));
	pair-> camera1 = c1;
	pair-> camera2 = c2;
	pair-> trueLabel = -1;

	return pair;

}

List * createNegativePairs(Clique ** cliqueIndex,int numOfcliques){

	List * differentCameras = createList();
	int negativeClique_position=-1;

	for(int i=0;i<numOfcliques;i++){

		List * Clique = cliqueIndex[i]->set;
		listNode * clique_node = Clique->firstNode;

		while(clique_node != NULL){
	
			for(int c=0;  c<cliqueIndex[i]->numOfUnique_negativeCliques;  c++){

				negativeClique_position = cliqueIndex[i]->unique_negativeCliques[c];

				List * NegClique = cliqueIndex[negativeClique_position]->set;
				listNode * negclique_node = NegClique->firstNode;

				while(negclique_node != NULL){

					CamerasPair * pair = createPair((CamSpec*)clique_node->data,(CamSpec*)negclique_node->data);

					insert_toList(differentCameras ,(void*)pair);
					negclique_node = negclique_node->nextNode;
				}
			}
			clique_node = clique_node->nextNode;
		}
	}

	return differentCameras;
}

/* ---------------------- Dataset ---------------------- */

CamerasPair ** createDataset(List * sameCameras,List * differentCameras,int * dataset_size){

	srand(time(NULL));
	int same = get_listSize(sameCameras),different = get_listSize(differentCameras);
	printf("Same Cameras:      %d\n", same);
	printf("Different Cameras: %d\n", different);
	*dataset_size = same+different;
	printf("Dataset size: %d\n",* dataset_size);

	CamerasPair ** Dataset = malloc(*dataset_size*sizeof(CamerasPair *));
	int i=0;

	listNode * samePair_node = sameCameras->firstNode;
	listNode * difPair_node = differentCameras->firstNode;
	

	while(difPair_node!=NULL || samePair_node!=NULL){

		if(difPair_node!=NULL)	
			setLabel(difPair_node->data,DIFFERENT_CAMERAS);

		if(samePair_node!=NULL)	
			setLabel(samePair_node->data,SAME_CAMERAS);

		if((rand()%3 == 0 && samePair_node!=NULL) || (difPair_node == NULL && samePair_node !=NULL)){
			Dataset[i] = (CamerasPair*)samePair_node->data;
			i++;
			if(samePair_node!=NULL)	
				samePair_node = samePair_node->nextNode;
		}

		if(difPair_node!=NULL){
			Dataset[i] = (CamerasPair*) difPair_node->data;
			i++;	
		}
		
		if(difPair_node!=NULL)	
			difPair_node = difPair_node->nextNode;
	}


	return Dataset;

}

void printDataset(CamerasPair ** Dataset,int dataset_size){

	FILE * dataset_file = fopen("DATASET.csv","w+");
	for(int i=0;i<dataset_size;i++)
		fprintf(dataset_file, "%s, %s, %d,  \n",((CamSpec*)Dataset[i]->camera1)->name,((CamSpec*)Dataset[i]->camera2)->name, Dataset[i]->trueLabel);

	fclose(dataset_file);
}


void setLabel(CamerasPair *  pair,int label){
	pair->trueLabel = label;
}

void deletePair(CamerasPair * pair){
	free(pair);
}

int stringComparator(const void * str1,const void * str2){
    return strcmp((char*) str1,(char*) str2);
}

void printCameraName(void * data,FILE * output){
	fprintf(output,"%s",((CamSpec*)data)->name);
}

