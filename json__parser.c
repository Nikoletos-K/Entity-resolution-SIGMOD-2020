#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>


char * clean_text(char * str){
	str++;
	str[strlen(str)-1] = '\0';	
	return str;
}

void read_jsonSpecs(char* filename){
	
	FILE * json_file = fopen(filename,"r");

	while(!feof(json_file)){
		char line[1024];
		fscanf(json_file,"%[^\n]\n",line);
		if(strcmp(line,"{") && strcmp(line,"}")){

			char * key = strtok(line,":");
			key++;
			key[strlen(key)-1] = '\0';	

			char * value = strtok(NULL,"");
			value = value+2;
			value[strlen(value)-2] = '\0';	

		}
	}
	fclose(json_file);

}



int main(){
	read_jsonSpecs("./data/camera_specs/2013_camera_specs/www.ebay.com/56414.json");
	return 0;
}