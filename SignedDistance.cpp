

#include "SignedDistance.hpp"

#include <stdio.h>
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
    return -1;
}

