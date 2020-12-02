#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../include/BloomFilter.h"

unsigned int size_of_bitArray(unsigned int BFsize){

	unsigned int bAsize = firstPrime(3*(BFsize),4*(BFsize));		/*first prime number after 3*size */
	return bAsize;
}


BF * createBF(unsigned int BFsize){

	BF * tempBF = malloc(sizeof(BF));
	tempBF->size = size_of_bitArray(BFsize);								/* I need prime(3*numofvoters) bit , so I allocate this number/8+1 * 8 bit */
	tempBF->bitArray = calloc((tempBF->size/8+1),sizeof(char));					/* Because char is 1 byte = 8 bit */

	return tempBF;
}

void setBit(BF * bf,int hash){

	int i = hash / 8;								/* Find in which position of the array it is */
	int pos = hash % 8;								/* Find in which position of the char it is the specific bit */
	unsigned int flag=1;							/* flag = 00000...00001 */
	flag = flag << pos; 								/* Shift it until I find the position of the bit */
	bf->bitArray[i] = bf->bitArray[i] | flag;			/* OR betxeen them , because 0 OR 1 = 1 and 0 OR 0 = 0 */
}


int checkBit(BF * bf,unsigned int hash){				/* Same method as above */
	
	int bin=0;
	int i=hash/8;
	int pos=hash%8;

	unsigned int flag=1;
	flag=flag << pos;

	if(bf->bitArray[i] & flag)
		bin=1;

	return bin;
}


void insertBF(BF * bf, void * identity){

		unsigned int hash1,hash2,hash3;

		hash1=hashFunction_1(identity)%bf->size;				/* Hash = hash1 % size , that gives me the position i xill enter the bit in the BF */
		setBit(bf,hash1);

		hash2=hashFunction_2(identity)%bf->size;				/* Same */
		setBit(bf,hash2);

		hash3=hashFunction_3(identity)%bf->size;			
		setBit(bf,hash3);

}


int checkBF(BF * bf, void * identity){

	int flag=0;
	unsigned int hash1,hash2,hash3;
	hash1=hashFunction_1(identity)%bf->size;
	hash2=hashFunction_2(identity)%bf->size;
	hash3=hashFunction_3(identity)%bf->size;	

	if(checkBit(bf,hash1))								/* It has to be all equal to 1 */
		if(checkBit(bf,hash2))
			if(checkBit(bf,hash3))
				flag=1;
	

	return flag;	

}

void destroyBF(BF * bf){

	free(bf->bitArray);
	free(bf);
}


unsigned int firstPrime(unsigned int min,unsigned int max){

	int flag;
	unsigned int div,n;
	for(n=min;n<max;n++){
		if(n%2==0) continue;
		if(n%3!=0){													 
			flag=1;
			div=5;														
			while(flag==1 && div*div<=n){									
				if(n%div==0 || n%(div+2)==0)	flag=0;			
				else	div=div+6;								  												
			}	
			if(flag)	break;									
		}	
	}
	return n;	
}


unsigned int hashFunction_1(void * inkey){

	int key = *(int*)inkey;
	key = (~key) + (key << 18); // key = (key << 18) - key - 1;
	key = key ^ (key >> 31);
	key = key * 21; // key = (key + (key << 2)) + (key << 4);
	key = key ^ (key >> 11);
	key = key + (key << 6);
	key = key ^ (key >> 22);
	return (int) key;
}


unsigned int hashFunction_2(void * inkey){

	int key = *(int*)inkey;
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	return key;
}

unsigned int hashFunction_3(void * inkey){

	int key = *(int*)inkey;
	key = (~key) + (key << 18); // key = (key << 18) - key - 1;
	key = key ^ (key >> 31);
	key = key * 21; // key = (key + (key << 2)) + (key << 4);
	key = key ^ (key >> 11);
	key = key + (key << 6);
	key = key ^ (key >> 22);
	
	return (int) key;

}
