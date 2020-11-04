#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

#include "json.h"
#include "./HashTable/HashTable.h"


#define BUFFER 100

void read_json(char* filename);
void read_dir(char* nameOfDir,HashTable * ht);
int read_csv(char * filename);
int stringComparator(void * str1,void * str2);