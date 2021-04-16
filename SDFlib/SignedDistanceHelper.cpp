
#include "../Common.hpp"
#include "SignedDistanceHelper.hpp"

#include <dlfcn.h>

void (*UserConstructSignedDistance)();
void (*UserDestructSignedDistance)();

void *SDlib = 0;

void InitSignedDistance(char *filename)
{
	char cmd[128];
	// sprintf(cmd, "gcc -Wall -fPIC -shared %s SignedDistanceObjects.cpp -o SignedDistance.so", filename);
	// system("rm -f SignedDistance.so");
	sprintf(cmd, "rm -f SDFlib/SignedDistance.so; make -C SDFlib/ SignedDistance.so SDF_FILE=../%s", filename);
	printf("%s\n", cmd);
	system(cmd);

	void *SDlib = dlopen("SDFlib/SignedDistance.so", RTLD_NOW);
	if (!SDlib)
	{
		printf("Error loading DLL: %s\n", dlerror());
		exit(1);
	}

	UserSignedDistance = (float (*)(vec3))dlsym(SDlib, "SignedDistance");
	if (!UserSignedDistance)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
	UserConstructSignedDistance = (void (*)())dlsym(SDlib, "ConstructSignedDistance");
	if (!UserConstructSignedDistance)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
	UserDestructSignedDistance = (void (*)())dlsym(SDlib, "DestructSignedDistance");
	if (!UserDestructSignedDistance)
	{
		printf("Error loading DLL function: %s\n", dlerror());
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	// dlclose(SDlib);
}


