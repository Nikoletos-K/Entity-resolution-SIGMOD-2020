#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/acutest.h"
#include "./../include/utils.h"

void test_jsonInfo(void){

	char * key = "key";
	jsonInfo * ji = initializeJsonInfo(key);

	/* checking structs pointer and data initialization */
	TEST_ASSERT(ji!=NULL);
	TEST_ASSERT(ji->numOfvalues == 0);
	TEST_ASSERT(ji->value != NULL);
	TEST_ASSERT(ji->key != NULL);
	TEST_ASSERT(strcmp(ji->key,key) == 0);

	char * value = "value";
	char * string_exampleArray[10] = {"i","a","c","d","b","f","g","e","h","j"};
	
	/* inserting values to its array */
	for(int i=0;i<10;i++){
	
		ji->value = addValue(ji,string_exampleArray[i]);
	
		TEST_ASSERT(ji->value!=NULL);
		TEST_ASSERT(ji->numOfvalues == i+1);
	}

	/* chencking if insertion is done */ 
	for(int i=0;i<10;i++)
		TEST_ASSERT(strcmp(string_exampleArray[i],ji->value[i]) == 0);

	deleteJsonInfo(ji);
}

void test_CamSpec(void){

	char * name = "camera_1";
	int arrayPosition = 0;

	/* checking structs pointer and data initialization */
	CamSpec* cs = createCamSpec(name,arrayPosition);
	TEST_ASSERT(cs!=NULL);
	TEST_ASSERT(cs->numOfSpecs == 0);
	TEST_ASSERT(cs->infoArray != NULL);
	TEST_ASSERT(cs->arrayPosition == arrayPosition);
	TEST_ASSERT(strcmp(cs->name,name) == 0);	
	TEST_ASSERT(cs->set != NULL);
	TEST_ASSERT(get_listSize(cs->set) == 1);

	char * key_exampleArray[10] = {"k0","k1","k2","k3","k4","k5","k6","k7","k8","k9"};
	char * value_exampleArray[10] = {"v0","v1","v2","v3","v4","v5","v6","v7","v8","v9"};

	/* inserting json specs */
	for(int i=0;i<10;i++){
		cs = addJsonInfo(cs,key_exampleArray[i]);
		for(int j=0;j<10;j++)
			cs = addValuetoCS(cs,value_exampleArray[j]);
	}

	/* chencking if insertion is done */ 
	for(int i=0;i<10;i++){

		TEST_ASSERT(cs->numOfSpecs == 10);
		TEST_ASSERT(strcmp(key_exampleArray[i],cs->infoArray[i]->key) == 0);

		for(int j=0;j<10;j++){
			TEST_ASSERT(cs->infoArray[i]->numOfvalues == 10);
			TEST_ASSERT(strcmp(value_exampleArray[j],cs->infoArray[i]->value[j]) == 0);
		}
	}

	destroyCamSpec(cs);
}

void test_jsonParser(void){	

	/* json file in the current directory */
	char  test_json_file[20] = "json_test_file.json";
	int arrayPosition = 0;
	char * name = "camera_1";

	CamSpec* cs = createCamSpec(name,arrayPosition);
	cs = read_jsonSpecs(test_json_file,cs);
	int true_numOfKeys = 5;
	char * key_exampleArray[5] = {"<page title>", "brand name","builtin flash","camera modes","exposure control"};

	TEST_ASSERT(cs->numOfSpecs == true_numOfKeys);

	for(int i=0;i<true_numOfKeys;i++){

		/* five rows of json */
		/* checking if stored data match with json initial data */
		switch(i){
			case 0:
				TEST_ASSERT(cs->infoArray[i]->numOfvalues==1);
				TEST_ASSERT(strcmp(cs->infoArray[i]->key,key_exampleArray[i]) == 0);
				TEST_ASSERT(strcmp(cs->infoArray[i]->value[0],"Polaroid Is426") == 0);
				break;
			case 1:
				TEST_ASSERT(cs->infoArray[i]->numOfvalues==1);
				TEST_ASSERT(strcmp(cs->infoArray[i]->key,key_exampleArray[i]) == 0);
				TEST_ASSERT(strcmp(cs->infoArray[i]->value[0],"Polaroid") == 0);
				break;
			case 2:
				TEST_ASSERT(cs->infoArray[i]->numOfvalues==1);
				TEST_ASSERT(strcmp(cs->infoArray[i]->key,key_exampleArray[i]) == 0);
				TEST_ASSERT(strcmp(cs->infoArray[i]->value[0],"Yes") == 0);			
				break;
			case 3:
				TEST_ASSERT(cs->infoArray[i]->numOfvalues==1);
				TEST_ASSERT(strcmp(cs->infoArray[i]->key,key_exampleArray[i]) == 0);
				TEST_ASSERT(strcmp(cs->infoArray[i]->value[0],"Scene Modes: Portrait Night") == 0);		
				break;
			case 4:
				TEST_ASSERT(cs->infoArray[i]->numOfvalues==1);
				TEST_ASSERT(strcmp(cs->infoArray[i]->key,key_exampleArray[i]) == 0);
				TEST_ASSERT(strcmp(cs->infoArray[i]->value[0],"Auto") == 0);
				break;
		}
	}

	destroyCamSpec(cs);
}

TEST_LIST = {
	{"json reading methods",test_jsonInfo},
	{"camera storing methods",test_CamSpec},
	{"json parser",test_jsonParser},
	{ NULL, NULL }
};