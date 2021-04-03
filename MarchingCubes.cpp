

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

float sdBox(vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d,0.0f));
}

float SignedDistance(vec3 p)
{
    return sdBox(p, vec3(0.5f));
}


#include <vector>

std::vector<vec3> MarchingCubes()
{
    
}

