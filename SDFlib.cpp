
#include "Common.hpp"
#include "SDFlib.hpp"


vec3 translate(vec3 pos, vec3 move)
{
	return pos - move;
}

float sdSphere(vec3 pos, float r)
{
	return length(pos) - r;
}

float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d, vec3(0.0f)));
}

float sdBoxFast(vec3 pos, vec3 l)
{
	vec3 b = abs(pos) - l;
	return max(max(b.x, b.y), b.z);
}


float sdUnion(float d0, float d1)
{
	return min(d0, d1);
}

float sdInter(float d0, float d1)
{
	return max(d0, d1);
}

float sdCappedCylinder(vec3 pos, float h, float r)
{
	float l = sqrt(pow(pos.x,2.0f) + pow(pos.z,2.0f));
	float dx = abs(l) - r;
	float dy = abs(pos.y) - h;
	return min(max(dx,dy),0.0f) + sqrt(pow(max(dx,0.0f),2.0f) + pow(max(dy,0.0f),2.0f));
}




//== Objects ==

SDF3 Sphere(float radius)
{
	return SDF3(new SDSphere(radius));
}
SDF3 SphereD(float diameter)
{
	return SDF3(new SDSphere(diameter/2.0f));
}
SDF3 Box(vec3 size)
{
	return SDF3(new SDBox(size));
}
SDF3 Cube(float size)
{
	return SDF3(new SDBox(vec3(size)));
}
SDF3 Cylinder(float height, float radius)
{
	return SDF3(new SDCylinder(height, radius));
}


//== Modifiers ==

SDF3 Translate(SDF3 object, vec3 move)
{
	return SDF3(new SDTranslate(object, move));
}
SDF3 Union(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDUnion(object1, object2));
}
SDF3 Difference(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDDifference(object1, object2));
}
SDF3 Intersect(SDF3 object1, SDF3 object2)
{
	return SDF3(new SDIntersect(object1, object2));
}

