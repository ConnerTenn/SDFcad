

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "CubeTables.hpp"

#include "MarchingCubes.hpp"

float sdBox(vec3 p, vec3 b)
{
	vec3 d = abs(p) - b;
	return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d,0.0f));
}

float SignedDistance(vec3 p)
{
	return sdBox(p, vec3(0.5f));
}



std::vector<vec3> MarchingCubes()
{
	std::vector<vec3> triangleList;

	const float dimensions = 5.0f;
	const float resolution = 100.0f;

	const int bounds = dimensions*resolution;
	for (int x=-bounds; x<=bounds; x++)
	{
		for (int y=-bounds; y<=bounds; y++)
		{
			for (int z=-bounds; z<=bounds; z++)
			{
				vec3 pos = vec3(x/resolution, y/resolution, z/resolution);

				TRIANGLE triangles[5];
				GRIDCELL grid;
				grid.p[0] = vec3(pos.x-0.5, pos.y-0.5, pos.z-0.5);
				grid.p[1] = vec3(pos.x+0.5, pos.y-0.5, pos.z-0.5);
				grid.p[2] = vec3(pos.x+0.5, pos.y-0.5, pos.z+0.5);
				grid.p[1] = vec3(pos.x-0.5, pos.y-0.5, pos.z+0.5);

				grid.p[0] = vec3(pos.x-0.5, pos.y+0.5, pos.z-0.5);
				grid.p[1] = vec3(pos.x+0.5, pos.y+0.5, pos.z-0.5);
				grid.p[2] = vec3(pos.x+0.5, pos.y+0.5, pos.z+0.5);
				grid.p[1] = vec3(pos.x-0.5, pos.y+0.5, pos.z+0.5);


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

