


#include "SignedDistance.hpp"

#include <stdio.h>

#if PYTHON

/*
https://www.linuxjournal.com/article/8497
https://python.readthedocs.io/en/stable/extending/embedding.html#compiling
*/

#include <Python.h>

PyObject *PySignedDistance = 0;

void InitSignedDistance(const char *filename)
{
	Py_Initialize();
	FILE *file = fopen(filename, "r+");
	if (file == 0)
	{
		printf("Error opening %s\n", filename);
		return;
	}
	PyRun_SimpleFileEx(file, filename, 1);

	PyObject *mainModule = PyImport_AddModule("__main__");
	PyObject *globalDict = PyModule_GetDict(mainModule);

	// Extract a reference to the function "func_name"
	// from the global dictionary
	PySignedDistance = PyDict_GetItemString(globalDict, "SignedDistance");

}

void ShutdownSignedDistance()
{
	Py_Finalize();
}

float SignedDistance(vec3 pos)
{
	PyObject *pyReturn = PyObject_CallFunction(PySignedDistance, "fff", pos.x, pos.y, pos.z);
	if (!pyReturn)
	{
		PyErr_Print();
		exit(1);
	}
	float value = PyFloat_AsDouble(pyReturn);//PyFloat_AS_DOUBLE(pyReturn);
	Py_DECREF(pyReturn);

	return value;
}

#elif PYPY

/*
https://doc.pypy.org/en/improve-docs/embedding.html
*/

#include "include/PyPy.h"

char *pyfile;

float (*PySD)(float, float, float);
void callback(float (*func)(float, float, float))
{
	PySD = func;
}

void InitSignedDistance(const char *filename)
{
	rpython_startup_code();
	pypy_setup_home((char *)"/opt/pypy/bin/", 1);

	FILE *file = fopen("SignedDistance.py", "r");
	fseek(file, 0, SEEK_END);
	int filelen = ftell(file);
	fseek(file, 0, SEEK_SET);

	pyfile = (char *)malloc(filelen+1);
	fread(pyfile, 1, filelen, file); 
	pyfile[filelen] = 0;

	fclose(file);


	int res = pypy_execute_source_ptr((char*)pyfile, (void *)callback);
	if (res) 
	{
		printf("Error calling pypy_execute_source_ptr!\n");
		exit(1);
	}
}

void ShutdownSignedDistance()
{
	free(pyfile);
}


float SignedDistance(vec3 pos)
{
	return PySD(pos.x, pos.y, pos.z);
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
