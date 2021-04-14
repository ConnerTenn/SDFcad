
#ifndef _SDF_LIB_H_
#define _SDF_LIB_H_

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



vec3 translate(vec3 pos, vec3 move);
float sdSphere(vec3 pos, float r);
float sdBox(vec3 p, vec3 b);
float sdBoxFast(vec3 pos, vec3 l);
float sdCappedCylinder(vec3 pos, float h, float r);

float sdUnion(float d0, float d1);
float sdInter(float d0, float d1);



extern "C"
{
// == User Defined ==
void ConstructSignedDistance();
void DestructSignedDistance();
float SignedDistance(vec3 pos);
// == End User Defined ==

//Exported interface to the signed distance marching cubes implementation
float *MarchingCubes(unsigned int *numEntries);

}

#endif
