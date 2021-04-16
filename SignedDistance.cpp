
#include "SDFlib/SDFlib.hpp"

SDF3 Object;

void ConstructSignedDistance()
{
	SDF3 cylinder = Cylinder(2, 0.5);
	SDF3 cylinderX = Rotate(cylinder, TO_RAD(90), vec3(1,0,0));
	SDF3 cylinderZ = Rotate(cylinder, TO_RAD(90), vec3(0,0,1)); 

	Object = (Sphere(1) & Cube(1.5)) - (cylinder + cylinderX + cylinderZ);
}

void DestructSignedDistance()
{
	// Object.~SDF3();
}

float SignedDistance(vec3 pos)
{
	return Object(pos);
}
