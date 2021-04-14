
#include "Common.hpp"
#include "SignedDistanceHelper.hpp"

#include <dlfcn.h>

float (*SD)(float, float, float);

void *SDlib = 0;

void InitSignedDistance(const char *filename)
{
	char cmd[128];
	sprintf(cmd, "gcc -Wall -fPIC -shared %s SignedDistanceObjects.cpp -o SignedDistance.so", filename);
	printf("%s\n", cmd);
	system(cmd);

	void *SDlib = dlopen("./SignedDistance.so", RTLD_NOW);
	if (!SDlib)
	{
		printf("Error loading DLL: %s\n", dlerror());
		exit(1);
	}
	SD = (float (*)(float, float, float))dlsym(SDlib, "SignedDistance");
	if (!SD)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	// dlclose(SDlib);
}

float SignedDistance(vec3 pos)
{
	return SD(pos.x, pos.y, pos.z);
}



