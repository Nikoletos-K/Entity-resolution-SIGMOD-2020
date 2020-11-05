#pragma once

#include "./json.h"
#include "./HashTable.h"


#define BUFFER 100

void read_json(char* filename);
void read_dir(char* nameOfDir,HashTable * ht);
int read_csv(char * filename);
int stringComparator(const void * str1,const void * str2);