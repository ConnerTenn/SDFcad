
#include "Common.hpp"
#include "SignedDistanceHelper.hpp"
#include "StlWriter.hpp"

int main()
{
	struct timespec t1 = GetTime();
	
	InitSignedDistance("SignedDistance.cpp");

	std::cout << "Generating Marching Cubes...\n";
	unsigned int numEntries;
	ConstructSignedDistance();
	float *vertexData = MarchingCubes(&numEntries);
	DestructSignedDistance();
	std::cout << "Marching Cubes generated (" << numEntries/3 << " vertices) (" << (numEntries/3)/3 << " triangles)\n";

	WriteSTL(vertexData, numEntries/3/3, (char *)"mesh.stl");

	struct timespec t2 = GetTime();
	
	printf("Total Time: ");
	PrintDuration(t1, t2);
	printf("\n");

	return 0;
}