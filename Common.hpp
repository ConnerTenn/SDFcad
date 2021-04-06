
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <time.h>
#include <stdio.h>

struct timespec GetTime();

void PrintDuration(struct timespec t1, struct timespec t2);

int ipow(int a, int b);

#endif
