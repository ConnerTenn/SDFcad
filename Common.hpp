
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PI (3.14159265358979323)
#define TAU (2*PI)
#define TO_RAD(degree) ((degree)*TAU/360.0f)
#define TO_DEG(rad) ((rad)*360.0f/TAU)

typedef struct timespec Time;

Time GetTime();
std::string DurationString(Time t1, Time t2);

int ipow(int a, int b);

#endif
