
#ifndef __SIGNED_DISTANCE_HELPER_H_
#define __SIGNED_DISTANCE_HELPER_H_

#include "SDFlib.hpp"

void InitSignedDistance(char *filename);
void ShutdownSignedDistance();

extern float (*UserSignedDistance)(vec3);
extern void (*UserConstructSignedDistance)();
extern void (*UserDestructSignedDistance)();

#endif
