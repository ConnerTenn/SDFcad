
#include "Common.hpp"
#include "SignedDistanceHelper.hpp"

#include <dlfcn.h>

float *(*MarchingCubes)(unsigned int *numEntries);
void (*ConstructSignedDistance)();
void (*DestructSignedDistance)();

void *SDlib = 0;

void InitSignedDistance(const char *filename)
{
	char cmd[128];
	// sprintf(cmd, "gcc -Wall -fPIC -shared %s SignedDistanceObjects.cpp -o SignedDistance.so", filename);
	// system("rm -f SignedDistance.so");
	sprintf(cmd, "rm -f SignedDistance.so; make SignedDistance.so SDF_FILE=%s", filename);
	printf("%s\n", cmd);
	system(cmd);

	void *SDlib = dlopen("./SignedDistance.so", RTLD_NOW);
	if (!SDlib)
	{
		printf("Error loading DLL: %s\n", dlerror());
		exit(1);
	}

	MarchingCubes = (float *(*)(unsigned int *))dlsym(SDlib, "MarchingCubes");
	if (!MarchingCubes)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
	ConstructSignedDistance = (void (*)())dlsym(SDlib, "ConstructSignedDistance");
	if (!MarchingCubes)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
	DestructSignedDistance = (void (*)())dlsym(SDlib, "DestructSignedDistance");
	if (!MarchingCubes)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	// dlclose(SDlib);
}


