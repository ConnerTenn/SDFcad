
#include "StlWriter.hpp"

#include <glm/fwd.hpp>
using namespace glm;

#include <stdio.h>
#include <string.h>


void WriteSTL(float *vertexData, int numTriangles, char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file==0)
	{
		printf("Error opening file \"%s\" for writing\n", filename);
	}

	u8 header[80];
	memset(header, 0, 80);
	fwrite(header, 1, 80, file);

	

	fclose(file);
}

