
/*
https://www.linuxjournal.com/article/8497
https://python.readthedocs.io/en/stable/extending/embedding.html#compiling
*/


#include "SignedDistance.hpp"

#include <stdio.h>

#ifdef PYTHON

#include <Python.h>
#include <dlfcn.h>

float (*PySignedDistance)(float x, float y, float z);

void InitSignedDistance(const char *filename)
{
	void *myso = dlopen("./PySignedDistance.so", RTLD_NOW);
	if (myso == 0) { fprintf(stderr, "Error: could not load library PySignedDistance.so: %s\n", dlerror()); exit(1); }

	PyObject *(*PyInit_PySignedDistance)() = (PyObject *(*)())dlsym(myso, "PyInit_PySignedDistance");
	if (PyInit_PySignedDistance == 0) { fprintf(stderr, "Error: could not load function PyInit_PySignedDistance: %s\n", dlerror()); exit(1); }

	PySignedDistance = (float (*)(float, float, float))dlsym(myso, "SignedDistance");
	if (PySignedDistance == 0) { fprintf(stderr, "Error: could not load function SignedDistance: %s\n", dlerror()); exit(1); }


	if (PyImport_AppendInittab("PySignedDistance", PyInit_PySignedDistance) == -1) 
	{
		fprintf(stderr, "Error: could not extend in-built modules table\n");
		exit(1);
	}
	
	Py_Initialize();
	
	PyObject *pmodule = PyImport_ImportModule("PySignedDistance");
	if (!pmodule) 
	{
		PyErr_Print(); 
		fprintf(stderr, "Error: could not import module 'PySignedDistance'\n");
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	Py_Finalize();
}

float SignedDistance(vec3 pos)
{
	return PySignedDistance(pos.x, pos.y, pos.z);
}

#else


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

#include "SDF.frag"

#endif
