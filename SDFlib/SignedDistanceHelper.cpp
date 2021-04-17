
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
	std::cout << cmd << "\n";
	system(cmd);

	SDlib = dlopen("SDFlib/SignedDistance.so", RTLD_NOW);
	if (!SDlib)
	{
		std::cout << "Error loading DLL: " << dlerror() << "\n";
		exit(1);
	}

	UserSignedDistance = (float (*)(vec3))dlsym(SDlib, "SignedDistance");
	if (!UserSignedDistance)
	{
		std::cout << "Error loading DLL function: " << dlerror() << "\n";
		exit(1);
	}
	UserConstructSignedDistance = (void (*)())dlsym(SDlib, "ConstructSignedDistance");
	if (!UserConstructSignedDistance)
	{
		std::cout << "Error loading DLL function: " << dlerror() << "\n";
		exit(1);
	}
	UserDestructSignedDistance = (void (*)())dlsym(SDlib, "DestructSignedDistance");
	if (!UserDestructSignedDistance)
	{
		std::cout << "Error loading DLL function: " << dlerror() << "\n";
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	dlclose(SDlib);
	SDlib = 0;
}


