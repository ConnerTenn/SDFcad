
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


