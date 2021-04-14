
#include "SignedDistanceObjects.hpp"


float SD(vec3 pos)
{

	float f = max(sdSphere(pos, 1), sdBoxFast(pos, vec3(1.5/2,1.5/2,1.5/2)));

	vec3 posz = vec3(pos.x, pos.z, pos.y);
	vec3 posx = vec3(pos.y, pos.x, pos.z);
	float t = min(sdCappedCylinder(posx, 2, 0.5f), min(sdCappedCylinder(pos, 2, 0.5f), sdCappedCylinder(posz, 2, 0.5f)));
	return max(f, -t);
}
