#pragma once

#include "./json.h"
#include "./DisJointSet.h"

#define BUFFER 1024
#define SAME_CAMERAS 1

/* Body of entity resolution */
CamSpec * read_jsonSpecs(char* filename,CamSpec * cs);
CamSpec ** read_dir(char* nameOfDir,HashTable * ht,CamSpec ** camArray,int *array_position);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht,DisJointSet * djSet);
void printCameraName(void * data,FILE * output);
void printPairs(DisJointSet * djSet,int print_stdout);

/* utils */
int stringComparator(const void * str1,const void * str2);

/* Cantor functions */
int CantorEncode(int num1,int num2);
int CantorDecode(int cantor_number,int* num1,int* num2);
