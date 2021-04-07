
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PYTHON 0
#define PYPY 1


#if PYTHON || PYPY
void InitSignedDistance(const char *filename);
void ShutdownSignedDistance();
#endif

float SignedDistance(vec3 pos);



float *MarchingCubes(unsigned int *numEntries);
