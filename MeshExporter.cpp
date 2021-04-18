
#include "Common.hpp"
#include "SDFlib/SignedDistanceHelper.hpp"
#include "SDFlib/StlWriter.hpp"

#include <string.h>

int main(int argc, char **argv)
{
	char *filename = (char *)"mesh.stl";
	char *cppSource = (char *)"SignedDistance.cpp";

	//Parse Options
	for (int i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-o")==0)
		{
			filename = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "-c")==0)
		{
			cppSource = argv[i+1];
			i++;
		}
		else
		{
			std::cout << "Error: Unknown option " << argv[i] << "\n";
		}
	}


	Time t1 = GetTime();
	
	//Initialize the signed distance library
	InitSignedDistance(cppSource);

	std::cout << "Generating Marching Cubes...\n";

	unsigned int numEntries;
	UserConstructSignedDistance();
	float *vertexData = GenerateMesh(&numEntries, 1.6, 0.01);
	UserDestructSignedDistance();

	//Print stats
	std::cout << "Marching Cubes generated (" << numEntries/3 << " vertices) (" << (numEntries/3)/3 << " triangles)\n";

	//Write the generated triangles to the stl file
	WriteSTL(vertexData, numEntries/3/3, filename);

	Time t2 = GetTime();

	//Print timing
	std::cout << "Total Time: " << DurationString(t1, t2) << "\n";

	return 0;
}