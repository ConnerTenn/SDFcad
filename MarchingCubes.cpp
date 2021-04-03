
#include <iostream>
#include <stdlib.h>

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



float *MarchingCubes(unsigned int *numEntries)
{
	const float dimensions = 2.0f;
	const float resolution = 40.0f;

	const unsigned int dimres = dimensions*resolution;
	const unsigned int maxSize = (2*dimres+1)*(2*dimres+1)*(2*dimres+1) * (3*8);

	unsigned int bufferSize = maxSize;
	float *vertexData = (float *)malloc(sizeof(float)*bufferSize);
	*numEntries = 0;

	typedef float Arr3D[dimres*2+1][dimres*2+1][dimres*2+1];
	Arr3D *distanceArr = (Arr3D *)malloc(sizeof(Arr3D));

	volatile const int bounds = dimres;
	//Sample each point in the world space
	for (int x=-bounds; x<=bounds; x++)
	{
		for (int y=-bounds; y<=bounds; y++)
		{
			for (int z=-bounds; z<=bounds; z++)
			{
				vec3 pos = vec3(x/resolution, y/resolution, z/resolution);
				(*distanceArr)[x+bounds][y+bounds][z+bounds] = SignedDistance(pos);
			}
		}
	}

	//Run marching cubes algorithm
	for (int x=-bounds; x<bounds; x++)
	{
		for (int y=-bounds; y<bounds; y++)
		{
			for (int z=-bounds; z<bounds; z++)
			{
				vec3 pos = vec3(x/resolution, y/resolution, z/resolution);
				// std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";

				TRIANGLE triangles[5];
				GRIDCELL grid;
				grid.p[0] = vec3(pos.x,                pos.y, pos.z               );
				grid.p[1] = vec3(pos.x+1.0/resolution, pos.y, pos.z               );
				grid.p[2] = vec3(pos.x+1.0/resolution, pos.y, pos.z+1.0/resolution);
				grid.p[3] = vec3(pos.x,                pos.y, pos.z+1.0/resolution);

				grid.p[4] = vec3(pos.x,                pos.y+1.0/resolution, pos.z               );
				grid.p[5] = vec3(pos.x+1.0/resolution, pos.y+1.0/resolution, pos.z               );
				grid.p[6] = vec3(pos.x+1.0/resolution, pos.y+1.0/resolution, pos.z+1.0/resolution);
				grid.p[7] = vec3(pos.x,                pos.y+1.0/resolution, pos.z+1.0/resolution);


				grid.val[0] = (*distanceArr)[x+bounds  ][y+bounds][z+bounds  ];
				grid.val[1] = (*distanceArr)[x+bounds+1][y+bounds][z+bounds  ];
				grid.val[2] = (*distanceArr)[x+bounds+1][y+bounds][z+bounds+1];
				grid.val[3] = (*distanceArr)[x+bounds  ][y+bounds][z+bounds+1];

				grid.val[4] = (*distanceArr)[x+bounds  ][y+bounds+1][z+bounds  ];
				grid.val[5] = (*distanceArr)[x+bounds+1][y+bounds+1][z+bounds  ];
				grid.val[6] = (*distanceArr)[x+bounds+1][y+bounds+1][z+bounds+1];
				grid.val[7] = (*distanceArr)[x+bounds  ][y+bounds+1][z+bounds+1];

				int numtri = Polygonise(grid, 0.0, triangles);

				for (int i=0; i<numtri; i++)
				{
					vertexData[(*numEntries)++] = triangles[i].p[0].x;
					vertexData[(*numEntries)++] = triangles[i].p[0].y;
					vertexData[(*numEntries)++] = triangles[i].p[0].z;

					vertexData[(*numEntries)++] = triangles[i].p[1].x;
					vertexData[(*numEntries)++] = triangles[i].p[1].y;
					vertexData[(*numEntries)++] = triangles[i].p[1].z;

					vertexData[(*numEntries)++] = triangles[i].p[2].x;
					vertexData[(*numEntries)++] = triangles[i].p[2].y;
					vertexData[(*numEntries)++] = triangles[i].p[2].z;

					// if ((*numEntries) == bufferSize)
					// {
					// 	std::cout << "Realloc\n";
					// 	bufferSize += 1000*3*3;
					// 	vertexData = (float *)realloc(vertexData, sizeof(float)*bufferSize);
					// }
				}
			}
		}
	}

	free(distanceArr);

	return vertexData;
}

