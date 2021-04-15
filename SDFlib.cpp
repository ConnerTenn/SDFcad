
#include "Common.hpp"
#include "SDFlib.hpp"


//== Objects ==

SDF3 Sphere(float radius)
{
	return SDF3(new SDFSphere(radius));
}
SDF3 SphereD(float diameter)
{
	return SDF3(new SDFSphere(diameter/2.0f));
}
SDF3 Box(vec3 size)
{
	return SDF3(new SDFBox(size));
}
SDF3 Cube(float size)
{
	return SDF3(new SDFBox(vec3(size)));
}
SDF3 Cylinder(float height, float radius)
{
	return SDF3(new SDFCylinder(height, radius));
}


//== Modifiers ==

SDF3 Translate(SDF3 object, vec3 move)
{
	return SDF3(new SDFTranslate(object, move));
}
SDF3 Union(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDFUnion(object1, object2));
}
SDF3 Difference(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDFDifference(object1, object2));
}
SDF3 Intersect(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDFIntersect(object1, object2));
}
SDF3 Transform(SDF3 object, mat4 matrix)
{
	return SDF3(new SDTransform(object, matrix));
}
SDF3 Rotate(SDF3 object, float angle, vec3 axis)
{
	mat4 matrix = glm::rotate(mat4(1), angle, axis);
	return SDF3(new SDTransform(object, matrix));
}

