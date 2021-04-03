
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "CubeTables.hpp"

#include "MarchingCubes.hpp"

const float PI = 3.14159265359;
vec3 translate(vec3 pos, vec3 move)
{
	return pos - move; 
}
float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d,0.0f));
}
float sdSphere(vec3 pos, float r)
{
	return length(pos) - r;
}
float sdUnion(float d0, float d1)
{
	return min(d0, d1);
}
float sdInter(float d0, float d1)
{
	return max(d0, d1);
}

float SignedDistance(vec3 p)
{
	vec3 p1 = translate(p, vec3(0.6,0,0));
	vec3 p2 = translate(p, vec3(-0.6,0,0));

	float d0 = sdBox(p1, vec3(0.5));
	float d1 = sdSphere(p1, 0.6);
	float d2 = sdSphere(p2, 0.6);
	
	float d = sdInter(d1, d0) + 
		sin(p1.y*2*PI*10.0f)/50.0f + 
		sin(p1.x*2*PI*10.0f)/100.0f + 
		sin(p1.z*2*PI*10.0f)/100.0f;

	return sdUnion(d, d2);
}



std::vector<vec3> MarchingCubes()
{
	std::vector<vec3> triangleList;

	const float dimensions = 2.0f;
	const float resolution = 20.0f;

	const int bounds = dimensions*resolution;
	for (int x=-bounds; x<=bounds; x++)
	{
		for (int y=-bounds; y<=bounds; y++)
		{
			for (int z=-bounds; z<=bounds; z++)
			{
				vec3 pos = vec3(x/resolution, y/resolution, z/resolution);
				// std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";

				TRIANGLE triangles[5];
				GRIDCELL grid;
				grid.p[0] = vec3(pos.x-0.5/resolution, pos.y-0.5/resolution, pos.z-0.5/resolution);
				grid.p[1] = vec3(pos.x+0.5/resolution, pos.y-0.5/resolution, pos.z-0.5/resolution);
				grid.p[2] = vec3(pos.x+0.5/resolution, pos.y-0.5/resolution, pos.z+0.5/resolution);
				grid.p[3] = vec3(pos.x-0.5/resolution, pos.y-0.5/resolution, pos.z+0.5/resolution);

				grid.p[4] = vec3(pos.x-0.5/resolution, pos.y+0.5/resolution, pos.z-0.5/resolution);
				grid.p[5] = vec3(pos.x+0.5/resolution, pos.y+0.5/resolution, pos.z-0.5/resolution);
				grid.p[6] = vec3(pos.x+0.5/resolution, pos.y+0.5/resolution, pos.z+0.5/resolution);
				grid.p[7] = vec3(pos.x-0.5/resolution, pos.y+0.5/resolution, pos.z+0.5/resolution);


				for (int i=0; i<8; i++)
				{
					grid.val[i] = SignedDistance(grid.p[i]);
				}

				int numtri = Polygonise(grid, 0.0, triangles);

				for (int i=0; i<numtri; i++)
				{
					triangleList.push_back(triangles[i].p[0]);
					triangleList.push_back(triangles[i].p[1]);
					triangleList.push_back(triangles[i].p[2]);
				}
			}
		}
	}

	return triangleList;
}

