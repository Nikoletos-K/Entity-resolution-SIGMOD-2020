#include "./../include/acutest.h"			

#include "./../iclude/DisJointSet.h"

void test_create(void){
	int size=10;
	int* array = malloc(N * sizeof(*array));
	DisJointSet * set = DJSConstruct(size,&array);

	if(TEST_ASSERT(set != NULL)){

		if(TEST_ASSERT(set->rankArray != NULL)){

			for(int i=0;i<set->size;i++)
				TEST_ASSERT(set->rankArray[i] == 0);
		}

		if(TEST_ASSERT(set->parentArray != NULL)){

			for(int i=0;i<set->size;i++)
				TEST_ASSERT(set->parentArray[i] == i);
		}

		TEST_ASSERT(array!=NULL);
	}

	free(array);
	DJSDestructor(set);
} 

void test_union(void){
	int size=10;
	int x=1,y=2;
	int* array = malloc(N * sizeof(*array));
	DisJointSet * set = DJSConstruct(size,&array);

	DJSUnion(set,x,y);
	TEST_ASSERT(DJSFindParent(set,x) == DJSFindParent(set,y))

	free(array);
	DJSDestructor(set);
}

void test_getters(void){
	int* array = malloc(N * sizeof(*array));
	DisJointSet * set = DJSConstruct(size,&array);

	int x=0;

	TEST_ASSERT(DJSgetRank(set, x) == 0);
	TEST_ASSERT(DJSgetParent(set, x) == 0);

	free(array);
	DJSDestructor(set);
}

void test_setters(void){
	int* array = malloc(N * sizeof(*array));
	DisJointSet * set = DJSConstruct(size,&array);

	int x=0,int y=1;

	DJSsetParent(set, x, y);
	DJSsetRank(set, x, y);

	TEST_ASSERT(DJSgetRank(set, x) == 1);
	TEST_ASSERT(DJSgetParent(set, x) == 1);

	free(array);
	DJSDestructor(set);
}

TEST_SET = {
	{ "DJSConstruct", test_create },
	{ "DJSUnion", test_union },
	{ "DJSgetRank", test_getters },
	{ "DJSsetParent",test_setters}
	{ NULL, NULL } 
};