#pragma once

#include "./dictionary.h"

void createVectors(CamSpec ** camArray,int num_of_cameras);
void printVector(CamSpec ** camArray,int num_of_cameras);
float* concatVectors(float * vector1,float * vector2,int vectorSize);

