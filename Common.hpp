
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PI (3.14159265358979323)
#define TAU (2*PI)
#define TO_RAD(degree) ((degree)*TAU/360.0f)
#define TO_DEG(rad) ((rad)*360.0f/TAU)


struct timespec GetTime();

void PrintDuration(struct timespec t1, struct timespec t2);

int ipow(int a, int b);

#endif
