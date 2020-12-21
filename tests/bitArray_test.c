#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../include/acutest.h"			
#include "./../include/bitArray.h"


void test_create(void){
	int size = 10;
	BF* bitArray = createBF(size);

	TEST_ASSERT(bitArray != NULL);
	TEST_ASSERT(bitArray->size != 0);
	
	destroyBF(bitArray);
}

void check_and_set(void){
	int size = 10;
	BF* bitArray = createBF(size);
	int hash = 5;

	setBit(bitArray,hash);
	int bin = checkBit(bitArray, hash);
	TEST_ASSERT(bin == 1);

	
	destroyBF(bitArray);

}

TEST_LIST = {
	{ "createBF", test_create },
	{ "checkBit and setBit", check_and_set },
	{ NULL, NULL } 
};