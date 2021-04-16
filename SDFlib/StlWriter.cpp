
#include "StlWriter.hpp"

#include <stdio.h>
#include <string.h>

typedef struct __attribute__((__packed__))
{
	float Normal[3];
	float Vertex1[3];
	float Vertex2[3];
	float Vertex3[3];
	u16 Attribute;
	//No extra padding due to packed
} TriangleData;

vec3 CalculateNormal(vec3 p1, vec3 p2, vec3 p3)
{
	vec3 n1 = cross(p2-p1, p3-p1);
	return normalize(n1);
}

void WriteSTL(float *vertexData, u32 numTriangles, char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file==0)
	{
		printf("Error opening file \"%s\" for writing\n", filename);
	}

	//== Header ==
	u8 header[80];
	memset(header, 0, 80);
	fwrite(header, 1, 80, file);

	//Number of triangles
	fwrite(&numTriangles, sizeof(numTriangles), 1, file);

	//== Triangle Data ==
	for (u32 i=0; i<numTriangles; i++)
	{
		TriangleData data;

		//Copy all vertex data into the data structure
		memcpy(data.Vertex1, vertexData+i*3*3, 3*3*sizeof(float));

		vec3 normal = CalculateNormal(
			vec3(data.Vertex1[0], data.Vertex1[1], data.Vertex1[2]),
			vec3(data.Vertex2[0], data.Vertex2[1], data.Vertex2[2]),
			vec3(data.Vertex3[0], data.Vertex3[1], data.Vertex3[2])
			);

		data.Normal[0] = normal.x;
		data.Normal[1] = normal.y;
		data.Normal[2] = normal.z;
		data.Attribute = 0;

		fwrite(&data, sizeof(data), 1, file);
	}

	fclose(file);
}

