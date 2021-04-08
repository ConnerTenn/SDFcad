
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


void InitSignedDistance(const char *filename);
void ShutdownSignedDistance();
float SignedDistance(vec3 pos);



float *MarchingCubes(unsigned int *numEntries);
