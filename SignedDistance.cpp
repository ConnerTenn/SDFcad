
#include "SDFlib.hpp"

SDF3 Object;

void ConstructSignedDistance()
{
	SDF3 obj = Sphere(0.2);
	SDF3 obj2 = obj;
	Object = Translate(obj, vec3(0.5,0,0)) + 
		(Cylinder(0.45, 0.2) - Translate(Box(vec3(0.2)), vec3(0.1,0.1,0.1))) + 
		(Translate(obj2, vec3(0,0.5,0)) & Translate(obj2, vec3(0,0.6,0)));
}

void DestructSignedDistance()
{
	// Object.~SDF3();
}

float SignedDistance(vec3 pos)
{
	// float f = max(sdSphere(pos, 1), sdBoxFast(pos, vec3(1.5/2,1.5/2,1.5/2)));

	// vec3 posz = vec3(pos.x, pos.z, pos.y);
	// vec3 posx = vec3(pos.y, pos.x, pos.z);
	// float t = min(sdCappedCylinder(posx, 2, 0.5f), min(sdCappedCylinder(pos, 2, 0.5f), sdCappedCylinder(posz, 2, 0.5f)));
	// return max(f, -t);

	// std::cout << "\n\n\n";
	return Object(pos);
}
