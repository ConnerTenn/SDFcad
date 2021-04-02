#define VERSION #version 330 core
VERSION

//Input Data
in vec4 gl_FragCoord;
uniform mat4 ViewMat;

//Ouput data
out vec3 fragColor;

#include "SDF.frag"

// get distance in the world
float dist_field(vec3 p)
{
	//  p = sdRep( p, vec3( 4.0 ) );
	//  p = sdTwist( p, 3.0 );
	
	float d0 = sdBox(p, vec3(0.5));
	float d1 = sdSphere(p, 0.6);
	
	float d = sdInter(d1, d0) + sin(p.y*2*PI*10.0f)/100.0f + sin(p.x*2*PI*10.0f)/500.0f + sin(p.z*2*PI*10.0f)/500.0f;

	return d;
	//return d + sfDisp( p * 2.5 );
	//return sdUnion_s( d + sfDisp( p * 2.5 * sin( iTime * 1.01 ) ), d1, 0.1 );
}

// get ray direction
vec3 ray_dir(float fov, vec2 size, vec2 pos) 
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
bool ray_marching(vec3 dir, inout float depth, out int iter)
{
	depth = 0.0;
	float dist = 0.0;
	float dt = 0.0;
	for (int i=0; i<MaxIter && depth<=ClipFar; i++) 
	{
		vec3 v = (ViewMat * vec4(dir*depth,1)).xyz;

		dist = dist_field(v);
		if (dist < StopThreshold)
		{
			iter = i;
			return true;
		}

		depth += abs(dist);
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
	vec2 iResolution = vec2(1024, 768);

	// default ray dir
	vec3 dir = ray_dir(45.0, iResolution.xy, gl_FragCoord.xy );

	// ray marching
	float depth = 0;
	int iter = 0;
	if (!ray_marching(dir, depth, iter)) 
	{
		fragColor = vec3(0);
		return;
	}

	fragColor = vec3(1.0/depth + 2.0*iter/MaxIter);
}