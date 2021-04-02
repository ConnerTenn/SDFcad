
// math
const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;

// iq's distance function
float sdSphere(vec3 pos, float r)
{
	return length(pos) - r;
}

float sdBox(vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdUnion(float d0, float d1)
{
	return min(d0, d1);
}

float sdInter(float d0, float d1)
{
	return max(d0, d1);
}

float sdSub(float d0, float d1)
{
	return max(d0, -d1);
}

float sdUnion_s(float a, float b, float k)
{
	float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
	return mix(b, a, h) - k*h*(1.0-h);
}

float sfDisp(vec3 p)
{
	return sin(p.x)*sin(p.y)*sin(p.z);
}

vec3 sdTwist(vec3 p, float a)
{
	float c = cos(a*p.y);
	float s = sin(a*p.y);
	mat2  m = mat2(c,-s,s,c);
	return vec3(m*p.xz,p.y);
}

vec3 sdRep(vec3 p, vec3 c)
{
	return mod(p,c)-0.5*c;
}

