#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/acutest.h"
#include "./../include/dictionary.h"

HashTable * Dictionary;
size_t DictionarySize;
size_t VectorSize;  
dictNode ** DictionaryNodes;


void test_CamSpec(void){

	char * name = "camera_1";
	int arrayPosition = 0;

	/* checking structs pointer and data initialization */
	CamSpec* cs = createCamSpec(name,arrayPosition);
	TEST_ASSERT(cs!=NULL);
	TEST_ASSERT(cs->arrayPosition == arrayPosition);
	TEST_ASSERT(strcmp(cs->name,name) == 0);	
	TEST_ASSERT(cs->set != NULL);
	TEST_ASSERT(get_listSize(cs->set) == 1);


	destroyCamSpec(cs);
}


TEST_LIST = {
	{"camera storing methods",test_CamSpec},
	{ NULL, NULL }
};