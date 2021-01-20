#pragma once

typedef struct BF {

	char * bitArray;
	unsigned int size;

}BF;

unsigned int size_of_bitArray(unsigned int BFsize);

BF * createBF(unsigned int BFsize);

void setBit(BF * bf,int hash);

int checkBit(BF * bf,unsigned int hash);

void insertBF(BF * bf, void * identity);
int checkBF(BF * bf, void * identity);

void destroyBF(BF * bf);

unsigned int firstPrime(unsigned int min,unsigned int max);
unsigned int hashFunction_1(void * inkey);
unsigned int hashFunction_2(void * inkey);
unsigned int hashFunction_3(void * inkey);

