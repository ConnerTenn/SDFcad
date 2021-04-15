
#include "SDFlib.hpp"

void ConstructSignedDistance()
{
	// std::string a = "A";
	// std::string b = a;
	// b[0] = 'B';
	// std::cout << a << " " << b << "\n";
	_SDF3 *ptr;
	// _SDF3 **ptr1;
	// _SDF3 **ptr2;
	SDF3 obj2;
	{
	std::cout << "===\n";
	SDF3 obj = Sphere(0.2); //&SDSphere(0.2);
	// ptr1 = &obj.Object;
	std::cout << "\n";

	obj2 = obj;
	std::cout << "\n";

	obj = Sphere(0.4); //&SDSphere(0.4);
	// ptr2 = &obj.Object;
	std::cout << "===\n";
	}
	// std::cout << ptr1 << " " << ptr2 << "\n";
	std::cout << "---\n";
	// delete ptr;
}

void DestructSignedDistance()
{
}

float SignedDistance(vec3 pos)
{
	float f = max(sdSphere(pos, 1), sdBoxFast(pos, vec3(1.5/2,1.5/2,1.5/2)));

	vec3 posz = vec3(pos.x, pos.z, pos.y);
	vec3 posx = vec3(pos.y, pos.x, pos.z);
	float t = min(sdCappedCylinder(posx, 2, 0.5f), min(sdCappedCylinder(pos, 2, 0.5f), sdCappedCylinder(posz, 2, 0.5f)));
	return max(f, -t);
}
