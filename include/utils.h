#pragma once

#include "./json.h"



#define BUFFER 100
#define SAME_CAMERAS 1

void read_json(char* filename);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position);
int read_csv(char * filename);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet * djSet);