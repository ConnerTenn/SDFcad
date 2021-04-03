#define VERSION #version 400 core
VERSION

//Input Data
in vec4 gl_FragCoord;
uniform mat4 ViewMat;

//Ouput data
out vec3 fragColor;

#include "SDFprimitives.frag"

#include "SDF.frag"

// get ray direction
vec3 RayDir(float fov, vec2 size, vec2 pos) 
{
	vec2 xy = pos - size * 0.5;

	float cot_half_fov = tan( (90.0 - fov*0.5) * DEG_TO_RAD );	
	float z = (size.x * 0.5) * cot_half_fov;

	return normalize(vec3(xy, -z));
}

// ray marching
const int MaxIter = 256;
const float StopThreshold = 0.001;
const float ClipFar = 1000.0;

// ray marching
bool RayMarch(vec3 dir, inout float depth, out int iter)
{
	depth = 0.0;
	float dist = 0.0;
	float dt = 0.0;
	for (iter=0; iter<MaxIter && depth<=ClipFar; iter++) 
	{
		vec3 v = (ViewMat * vec4(dir*depth,1)).xyz;

		dist = SignedDistance(v);
		if (dist < StopThreshold)
		{
			return true;
		}

		//Divide by 4 to better handle non- smoothly decending functions
		//Reduces rendering glitches of odd shapes
		depth += abs(dist)/4.0;
	}

	return false;
}

// camera rotation : pitch, yaw
mat3 rotationXY(vec2 angle)
{
	vec2 c = cos(angle);
	vec2 s = sin(angle);
	
	return mat3(
		c.y      ,  0.0, -s.y,
		s.y * s.x,  c.x,  c.y * s.x,
		s.y * c.x, -s.x,  c.y * c.x
	);
}

void main()
{
	vec2 resolution = vec2(1024, 768);

	// default ray dir
	vec3 dir = RayDir(45.0, resolution.xy, gl_FragCoord.xy );

	// ray marching
	float depth = 0;
	int iter = 0;
	if (!RayMarch(dir, depth, iter)) 
	{
		fragColor = vec3(0.3*float(iter)/MaxIter);
		return;
	}

	fragColor = vec3(1.0/depth + float(iter)/MaxIter);
}

