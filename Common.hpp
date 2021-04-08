
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

struct timespec GetTime();

void PrintDuration(struct timespec t1, struct timespec t2);

int ipow(int a, int b);

#endif
