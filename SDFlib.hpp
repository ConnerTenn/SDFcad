
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


//User Defined
float SignedDistance(vec3 pos);


extern "C"
{

//Exported interface to the signed distance marching cubes implementation
float *MarchingCubes(unsigned int *numEntries);

//Exported wrapper for calling Signed Distance
inline float SignedDistanceExt(float x, float y, float z)
{
	return SignedDistance(vec3(x,y,z));
}

}

#endif
