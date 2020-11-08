/*  Based on book Introduction to Algorithms ,Thomas H. Cormen (p. 566 - Greek edition)*/
#pragma once
#include <stdio.h>

typedef struct DisJointSet {

	size_t size;
	int * rankArray;
	int * parentArray;
	void ** objectArray;

} DisJointSet;

/* Constructor */
DisJointSet * DSJConstruct(int set_size,void ** objectArray);

/* Methods */
void DSJUnion(DisJointSet * dsjSet,int x,int y);
void DSJConnect(DisJointSet * dsjSet,int x,int y);
int DSJFindParent(DisJointSet * dsjSet,int x);

/* Getters */
int DSJgetRank(DisJointSet * dsjSet,int x);
int DSJgetParent(DisJointSet * dsjSet,int x);

/* Setters */
void DSJsetParent(DisJointSet * dsjSet,int x,int newparent);
void DSJsetRank(DisJointSet * dsjSet,int x,int newrank);