#pragma once

typedef struct BloomFilter{

	char * bitArray;
	unsigned int size;

}BF;

unsigned int size_of_bitArray(unsigned int size);

BF * createBF(unsigned int size);

void setBit(BF * bf,int hash);

void insertBF(BF * bf, void * key);
int checkBF(BF * bf, void * key);

void destroyBF(BF * bf);

unsigned int firstPrime(unsigned int min,unsigned int max);
unsigned int hashFunction_1(void * key);
unsigned int hashFunction_2(void * key);
unsigned int hashFunction_3(void * key);

