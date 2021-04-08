
#include "SignedDistance.hpp"

#include <stdio.h>

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


