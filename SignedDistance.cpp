
#include "SignedDistance.hpp"

#include "Python.h"

PyObject *PySignedDistance = 0;

void InitSignedDistance(const char *filename)
{
    FILE *file = fopen(filename, "r");
    PyRun_SimpleFile(file, filename);

    fclose(file);

    PyObject *mainModule = PyImport_AddModule("__main__");
    PyObject *globalDict = PyModule_GetDict(mainModule);

    // Extract a reference to the function "func_name"
    // from the global dictionary
    PySignedDistance = PyDict_GetItemString(globalDict, "SignedDistance");
}

void ShutdownSignedDistance()
{
}

float SignedDistance(vec3 pos)
{
    return -1;
}
