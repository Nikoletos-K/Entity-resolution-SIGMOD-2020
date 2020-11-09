#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/times.h>
#include <unistd.h>

void read_jsonSpecs(char* filename){
	
	
	
	FILE * json_file = fopen(filename,"r");

	// char temp[2] = "\"";
	printf("%d\n", json_file == NULL);
	while(!feof(json_file)){
		char line[1024];
		fscanf(json_file,"%[^\n]\n",line);
		if(strcmp(line,"{\n") && strcmp(line,"}\n")){
			printf("%s\n",line );
		}


	}
	fclose(json_file);

}


int main(){
	read_jsonSpecs("./data/camera_specs/2013_camera_specs/www.ebay.com/56414.json");
	return 0;
}