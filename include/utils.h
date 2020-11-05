#pragma once

#include "./json.h"



#define BUFFER 100
#define SAME_CAMERAS 1

void read_json(char* filename);
void read_dir(char* nameOfDir,HashTable * ht);
int read_csv(char * filename);
HashTable * make_sets_from_csv(char * csvfile,HashTable * ht);