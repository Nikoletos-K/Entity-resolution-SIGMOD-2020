#pragma once

#include "./json.h"
#include "./DisJointSet.h"

#define BUFFER 1024
#define SAME_CAMERAS 1

CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position);
int read_csv(char * filename);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet * djSet);
void printCameraName(void * data,FILE * output);
void printPairs(DisJointSet * djSet,int print_stdout);