
#ifndef __SIGNED_DISTANCE_HELPER_H_
#define __SIGNED_DISTANCE_HELPER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void InitSignedDistance(const char *filename);
void ShutdownSignedDistance();

extern float *(*MarchingCubes)(unsigned int *numEntries);

#endif
