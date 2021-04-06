
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PYTHON


#ifdef PYTHON
void InitSignedDistance(const char *filename);
void ShutdownSignedDistance();
#endif

float SignedDistance(vec3 pos);



float *MarchingCubes(unsigned int *numEntries);
