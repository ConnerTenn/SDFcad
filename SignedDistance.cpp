
/*
https://www.linuxjournal.com/article/8497
https://python.readthedocs.io/en/stable/extending/embedding.html#compiling
*/


#include "SignedDistance.hpp"

#include <stdio.h>

#ifdef PYTHON

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
    float value = PyFloat_AsDouble(pyReturn);//PyFloat_AS_DOUBLE(pyReturn);
    Py_DECREF(pyReturn);

    return value;
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
