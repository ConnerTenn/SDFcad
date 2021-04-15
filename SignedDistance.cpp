
#include "SDFlib.hpp"

SDF3 Object;

void ConstructSignedDistance()
{
	// SDF3 obj = Sphere(0.2);
	// SDF3 obj2 = obj;
	// Object = Translate(obj, vec3(0.5,0,0)) + 
	// 	(Cylinder(0.45, 0.2) - Translate(Box(vec3(0.2)), vec3(0.1,0.1,0.1))) + 
	// 	(Translate(obj2, vec3(0,0.5,0)) & Translate(obj2, vec3(0,0.6,0)));
	SDF3 cylinder = Cylinder(2, 0.5);
	Object = (Sphere(1) & Cube(1.5/2)) - 
		(cylinder + Rotate(cylinder, TO_RAD(90), vec3(1,0,0)) + Rotate(cylinder, TO_RAD(90), vec3(0,0,1)));
	
}

void DestructSignedDistance()
{
	// Object.~SDF3();
}

float SignedDistance(vec3 pos)
{
	return Object(pos);
}
