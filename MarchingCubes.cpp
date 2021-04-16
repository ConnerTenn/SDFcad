
#include "Common.hpp"

#define INTERPOLATE
#include "CubeTables.hpp"
#undef INTERPOLATE

#include "SDFlib.hpp"



float *VertexData;
unsigned int VertDataSize;
unsigned int NumEntries;

#define BUFF_STEP_SIZE (100*1000*3*3)

float *Points = 0;

void MarchingCubes(vec3 pos, float step, int resolution)
{
	int res = resolution+1;
	pos = pos + vec3(-step/2.0f,-step/2.0f,-step/2.0f);


	typedef float points_t[res][res][res];
	if (!Points)
	{
		Points = (float *)malloc(sizeof(float)*res*res*res);
	}
	points_t *points = (points_t *)Points;
	(*points)[0][0][0] = 0; //Suppresses warning


	for (int i=0; i<res*res*res; i++)
	{
		int x = i/(res*res);
		int y = (i/res)%res;
		int z = i%res;
		vec3 point = pos+vec3(x*step/resolution, y*step/resolution, z*step/resolution);
		(*points)[x][y][z] = SignedDistance(point);
	}

	for (int i=0; i<resolution*resolution*resolution; i++)
	{
		int x = i/(resolution*resolution);
		int y = (i/resolution)%resolution;
		int z = i%resolution;
		// vec3 off = vec3(x+step/resolution, y+step/resolution, z+step/resolution);

		TRIANGLE triangles[5];
		GRIDCELL grid;
		grid.p[0] = pos+vec3((x  )*step/resolution, y*step/resolution, (z  )*step/resolution);
		grid.p[1] = pos+vec3((x+1)*step/resolution, y*step/resolution, (z  )*step/resolution);
		grid.p[2] = pos+vec3((x+1)*step/resolution, y*step/resolution, (z+1)*step/resolution);
		grid.p[3] = pos+vec3((x  )*step/resolution, y*step/resolution, (z+1)*step/resolution);

		grid.p[4] = pos+vec3((x  )*step/resolution, (y+1)*step/resolution, (z  )*step/resolution);
		grid.p[5] = pos+vec3((x+1)*step/resolution, (y+1)*step/resolution, (z  )*step/resolution);
		grid.p[6] = pos+vec3((x+1)*step/resolution, (y+1)*step/resolution, (z+1)*step/resolution);
		grid.p[7] = pos+vec3((x  )*step/resolution, (y+1)*step/resolution, (z+1)*step/resolution);


		grid.val[0] = (*points)[x  ][y][z  ];
		grid.val[1] = (*points)[x+1][y][z  ];
		grid.val[2] = (*points)[x+1][y][z+1];
		grid.val[3] = (*points)[x  ][y][z+1];

		grid.val[4] = (*points)[x  ][y+1][z  ];
		grid.val[5] = (*points)[x+1][y+1][z  ];
		grid.val[6] = (*points)[x+1][y+1][z+1];
		grid.val[7] = (*points)[x  ][y+1][z+1];

		int numtri = Polygonise(grid, 0.0, triangles);

		for (int i=0; i<numtri; i++)
		{
			VertexData[NumEntries++] = triangles[i].p[0].x;
			VertexData[NumEntries++] = triangles[i].p[0].y;
			VertexData[NumEntries++] = triangles[i].p[0].z;

			VertexData[NumEntries++] = triangles[i].p[1].x;
			VertexData[NumEntries++] = triangles[i].p[1].y;
			VertexData[NumEntries++] = triangles[i].p[1].z;

			VertexData[NumEntries++] = triangles[i].p[2].x;
			VertexData[NumEntries++] = triangles[i].p[2].y;
			VertexData[NumEntries++] = triangles[i].p[2].z;

			if (NumEntries+9 >= VertDataSize)
			{
				VertDataSize = VertDataSize*2.5;
				std::cout << "Realloc: " << sizeof(float)*VertDataSize/1000/1000 << "MB\n";
				VertexData = (float *)realloc(VertexData, sizeof(float)*VertDataSize);
			}
		}
	}
	
}


void BatchingMarch(float range, int numsteps, int resolution)
{
	for (int x=-numsteps/2; x<=numsteps/2; x++)
	{
		for (int y=-numsteps/2; y<=numsteps/2; y++)
		{
			for (int z=-numsteps/2; z<=numsteps/2; z++)
			{
				vec3 pos = vec3(x*range/numsteps, y*range/numsteps, z*range/numsteps);

				if (abs(SignedDistance(pos)) <= range/numsteps*1.735f)//*2.0f) //sqrt(3.0f))
				{
					MarchingCubes(pos, range/numsteps, resolution);
				}
			}
		}
	}
}


void RecursiveMarch(vec3 xyz, float step, int recurse)//, int depth)
{
	// for (int i=0; i<depth; i++)
	// {
	// 	std::cout << "  ";
	// }
	// std::cout << xyz.x << " " << xyz.y << " " << xyz.z << "\n";
	float dist = SignedDistance(xyz);

	if (abs(dist) <= step*1.735f && recurse)
	{
		RecursiveMarch(xyz+vec3(-step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3( step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3( step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3(-step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1);//, depth+1);

		RecursiveMarch(xyz+vec3(-step/4.0f,+step/4.0f,-step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3( step/4.0f,+step/4.0f,-step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3( step/4.0f,+step/4.0f, step/4.0f), step/2.0f, recurse-1);//, depth+1);
		RecursiveMarch(xyz+vec3(-step/4.0f,+step/4.0f, step/4.0f), step/2.0f, recurse-1);//, depth+1);
	}
	else
	{
		TRIANGLE triangles[5];
		GRIDCELL grid;
		grid.p[0] = xyz+vec3(-step/2.0f,-step/2.0f,-step/2.0f);
		grid.p[1] = xyz+vec3( step/2.0f,-step/2.0f,-step/2.0f);
		grid.p[2] = xyz+vec3( step/2.0f,-step/2.0f, step/2.0f);
		grid.p[3] = xyz+vec3(-step/2.0f,-step/2.0f, step/2.0f);

		grid.p[4] = xyz+vec3(-step/2.0f,+step/2.0f,-step/2.0f);
		grid.p[5] = xyz+vec3( step/2.0f,+step/2.0f,-step/2.0f);
		grid.p[6] = xyz+vec3( step/2.0f,+step/2.0f, step/2.0f);
		grid.p[7] = xyz+vec3(-step/2.0f,+step/2.0f, step/2.0f);


		grid.val[0] = SignedDistance(grid.p[0]);
		grid.val[1] = SignedDistance(grid.p[1]);
		grid.val[2] = SignedDistance(grid.p[2]);
		grid.val[3] = SignedDistance(grid.p[3]);

		grid.val[4] = SignedDistance(grid.p[4]);
		grid.val[5] = SignedDistance(grid.p[5]);
		grid.val[6] = SignedDistance(grid.p[6]);
		grid.val[7] = SignedDistance(grid.p[7]);

		int numtri = Polygonise(grid, 0.0, triangles);

		for (int i=0; i<numtri; i++)
		{
			VertexData[NumEntries++] = triangles[i].p[0].x;
			VertexData[NumEntries++] = triangles[i].p[0].y;
			VertexData[NumEntries++] = triangles[i].p[0].z;

			VertexData[NumEntries++] = triangles[i].p[1].x;
			VertexData[NumEntries++] = triangles[i].p[1].y;
			VertexData[NumEntries++] = triangles[i].p[1].z;

			VertexData[NumEntries++] = triangles[i].p[2].x;
			VertexData[NumEntries++] = triangles[i].p[2].y;
			VertexData[NumEntries++] = triangles[i].p[2].z;

			if (NumEntries+9 >= VertDataSize)
			{
				VertDataSize += BUFF_STEP_SIZE;
				std::cout << "Realloc: " << sizeof(float)*VertDataSize/1000/1000 << "MB\n";
				VertexData = (float *)realloc(VertexData, sizeof(float)*VertDataSize);
			}
		}
	}
}


//Faster than RecursiveMarch3 when SignedDistance is fast to compute (aka C++ implementation)
void RecursiveMarch2(vec3 xyz, float step, int recurse,
	float dist1, float dist2, float dist3, float dist4, float dist5, float dist6, float dist7, float dist8,
	bool d1En, bool d2En, bool d3En, bool d4En, bool d5En, bool d6En, bool d7En, bool d8En)
{
	float dist = SignedDistance(xyz);

	//abs(dist) <= sqrt(3)
	if (abs(dist) <= step*1.735f)//*2.0f) //sqrt(3.0f))
	{
		if (recurse)
		{
			RecursiveMarch2(xyz+vec3(-step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, dist1, 0,     0,     0,      0,    0,    dist, 0,        d1En,  false, false, false,  false, false, true,  false);
			RecursiveMarch2(xyz+vec3( step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,     dist2, 0,     0,      0,    0,    0,    dist,     false, d2En,  false, false,  false, false, false, true );
			RecursiveMarch2(xyz+vec3( step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,     0,     dist3, 0,      dist, 0,    0,    0,        false, false, d3En,  false,  true,  false, false, false);
			RecursiveMarch2(xyz+vec3(-step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,     0,     0,     dist4,  0,    dist, 0,    0,        false, false, false, d4En,   false, true,  false, false);

			RecursiveMarch2(xyz+vec3(-step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,    0,    dist, 0,         dist5, 0,     0,     0,     false, false, true,  false,  d5En,  false, false, false);
			RecursiveMarch2(xyz+vec3( step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,    0,    0,    dist,      0,     dist6, 0,     0,     false, false, false, true,   false, d6En,  false, false);
			RecursiveMarch2(xyz+vec3( step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, dist, 0,    0,    0,         0,     0,     dist7, 0,     true,  false, false, false,  false, false, d7En,  false);
			RecursiveMarch2(xyz+vec3(-step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,    dist, 0,    0,         0,     0,     0,     dist8, false, true,  false, false,  false, false, false, d8En );
		}
		else
		{
			TRIANGLE triangles[5];
			GRIDCELL grid;
			grid.p[0] = xyz+vec3(-step/2.0f,-step/2.0f,-step/2.0f);
			grid.p[1] = xyz+vec3( step/2.0f,-step/2.0f,-step/2.0f);
			grid.p[2] = xyz+vec3( step/2.0f,-step/2.0f, step/2.0f);
			grid.p[3] = xyz+vec3(-step/2.0f,-step/2.0f, step/2.0f);

			grid.p[4] = xyz+vec3(-step/2.0f, step/2.0f,-step/2.0f);
			grid.p[5] = xyz+vec3( step/2.0f, step/2.0f,-step/2.0f);
			grid.p[6] = xyz+vec3( step/2.0f, step/2.0f, step/2.0f);
			grid.p[7] = xyz+vec3(-step/2.0f, step/2.0f, step/2.0f);


			if (d1En) { grid.val[0] = dist1; } else { grid.val[0] = SignedDistance(grid.p[0]); }
			if (d2En) { grid.val[1] = dist2; } else { grid.val[1] = SignedDistance(grid.p[1]); }
			if (d3En) { grid.val[2] = dist3; } else { grid.val[2] = SignedDistance(grid.p[2]); }
			if (d4En) { grid.val[3] = dist4; } else { grid.val[3] = SignedDistance(grid.p[3]); }

			if (d5En) { grid.val[4] = dist5; } else { grid.val[4] = SignedDistance(grid.p[4]); }
			if (d6En) { grid.val[5] = dist6; } else { grid.val[5] = SignedDistance(grid.p[5]); }
			if (d7En) { grid.val[6] = dist7; } else { grid.val[6] = SignedDistance(grid.p[6]); }
			if (d8En) { grid.val[7] = dist8; } else { grid.val[7] = SignedDistance(grid.p[7]); }

			int numtri = Polygonise(grid, 0.0, triangles);

			for (int i=0; i<numtri; i++)
			{
				VertexData[NumEntries++] = triangles[i].p[0].x;
				VertexData[NumEntries++] = triangles[i].p[0].y;
				VertexData[NumEntries++] = triangles[i].p[0].z;

				VertexData[NumEntries++] = triangles[i].p[1].x;
				VertexData[NumEntries++] = triangles[i].p[1].y;
				VertexData[NumEntries++] = triangles[i].p[1].z;

				VertexData[NumEntries++] = triangles[i].p[2].x;
				VertexData[NumEntries++] = triangles[i].p[2].y;
				VertexData[NumEntries++] = triangles[i].p[2].z;

				if (NumEntries+9 >= VertDataSize)
				{
					VertDataSize += BUFF_STEP_SIZE;
					std::cout << "Realloc: " << sizeof(float)*VertDataSize/1000/1000 << "MB\n";
					VertexData = (float *)realloc(VertexData, sizeof(float)*VertDataSize);
				}
			}
		}
	}
}

#include <string.h>

typedef struct
{
	float Dist;
	bool Set;
} Distance;

class Array1D
{
public:
	Distance *Data;
	int XOff;
	Array1D(Distance *data, int xoff) :
		Data(data), XOff(xoff)
	{
	}

	Distance &operator[](int x)
	{
		return Data[x+XOff];
	}
};

class Array2D
{
private:
public:
	Distance *Data;
	int Stride;
	int Size;
	int XOff;
	bool IsSlice=false;

	Array2D()
	{
	}

public:
	Array2D(int size) :
		Stride(size), Size(size), XOff(size/2)
	{
		Data = new Distance[Size*Size];
		memset(Data, 0, sizeof(Distance)*Size*Size);
	}

	~Array2D()
	{
		if (!IsSlice)
		{
			delete[] Data;
		}
	}

public:
	Array1D operator[](int y)
	{
		return Array1D(&Data[(y+Size/2)*Stride], XOff);
	}

	//Problem with the calculations
	//Size never gets changed which means that the calculation becomes off
	//Might need another variable 'stride'
	Array2D TopLeft()
	{
		Array2D arr = Array2D();
		arr.Data = Data+(Size/2)*Stride;
		arr.Stride = Stride;
		arr.Size = Size/2+1;
		arr.XOff = XOff-Size/4;
		arr.IsSlice = true;
		return arr;
	}
	Array2D TopRight()
	{
		Array2D arr = Array2D();
		arr.Data = Data+(Size/2)*Stride;
		arr.Stride = Stride;
		arr.Size = Size/2+1;
		arr.XOff = XOff+Size/4;
		arr.IsSlice = true;
		return arr;
	}
	Array2D BottomLeft()
	{
		Array2D arr = Array2D();
		arr.Data = Data;//-(Size/4)*Stride;
		arr.Stride = Stride;
		arr.Size = Size/2+1;
		arr.XOff = XOff-Size/4;
		arr.IsSlice = true;
		return arr;
	}
	Array2D BottomRight()
	{
		Array2D arr = Array2D();
		arr.Data = Data;//-(Size/4)*Stride;
		arr.Stride = Stride;
		arr.Size = Size/2+1;
		arr.XOff = XOff+Size/4;
		arr.IsSlice = true;
		return arr;
	}
};

/*
x     y      z
back  bottom left
back  bottom right
front bottom right
front bottom left

back  top    left
back  top    right
front top    right
front top    left

n
0 |                               -                               |
1 |               -               |               -               |
2 |       -       |       -       |       -       |       -       |
3 |   -   |   -   |   -   |   -   |   -   |   -   |   -   |   -   |
4 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
5 |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|

#dashes   = 2**n
#dividers = 2**n + 1


*/

//Faster than RecursiveMarch2 when SignedDistance is slow to compute (aka python implementation)
void RecursiveMarch3(vec3 pos, float step, int recurse,
	Array2D top, Array2D bottom, Array2D left, Array2D right, Array2D front, Array2D back)
{
	int sidelen = ipow(2, recurse+1)+1;
	// printf("sidelen:%d\n", sidelen);
	Array2D xy = Array2D(sidelen);
	Array2D xz = Array2D(sidelen);
	Array2D yz = Array2D(sidelen);

	float dist = SignedDistance(pos);
	xy[0][0] = Distance{dist, true};

	//abs(dist) <= sqrt(3)
	if (abs(dist) <= step*1.735f)//*2.0f) //sqrt(3.0f))
	{
		if (recurse)
		{
			RecursiveMarch3(pos+vec3(-step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, xz.TopLeft(),     bottom.TopLeft(),     left.BottomLeft(),  yz.BottomLeft(),     xy.BottomLeft(),     back.BottomLeft() );
			RecursiveMarch3(pos+vec3( step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, xz.TopRight(),    bottom.TopRight(),    yz.BottomLeft(),    right.BottomLeft(),  xy.BottomRight(),    back.BottomRight());
			RecursiveMarch3(pos+vec3( step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, xz.BottomRight(), bottom.BottomRight(), yz.BottomRight(),   right.BottomRight(), front.BottomRight(), xy.BottomRight()  );
			RecursiveMarch3(pos+vec3(-step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, xz.BottomLeft(),  bottom.BottomLeft(),  left.BottomRight(), yz.BottomRight(),    front.BottomLeft(),  xy.BottomLeft()   );

			RecursiveMarch3(pos+vec3(-step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, top.TopLeft(),     xz.TopLeft(),     left.TopLeft(),  yz.TopLeft(),     xy.TopLeft(),     back.TopLeft() );
			RecursiveMarch3(pos+vec3( step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, top.TopRight(),    xz.TopRight(),    yz.TopLeft(),    right.TopLeft(),  xy.TopRight(),    back.TopRight());
			RecursiveMarch3(pos+vec3( step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, top.BottomRight(), xz.BottomRight(), yz.TopRight(),   right.TopRight(), front.TopRight(), xy.TopRight()  );
			RecursiveMarch3(pos+vec3(-step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, top.BottomLeft(),  xz.BottomLeft(),  left.TopRight(), yz.TopRight(),    front.TopLeft(),  xy.TopLeft()   );
		}
		else
		{
			TRIANGLE triangles[5];
			GRIDCELL grid;
			grid.p[0] = pos+vec3(-step/2.0f,-step/2.0f,-step/2.0f);
			grid.p[1] = pos+vec3( step/2.0f,-step/2.0f,-step/2.0f);
			grid.p[2] = pos+vec3( step/2.0f,-step/2.0f, step/2.0f);
			grid.p[3] = pos+vec3(-step/2.0f,-step/2.0f, step/2.0f);

			grid.p[4] = pos+vec3(-step/2.0f, step/2.0f,-step/2.0f);
			grid.p[5] = pos+vec3( step/2.0f, step/2.0f,-step/2.0f);
			grid.p[6] = pos+vec3( step/2.0f, step/2.0f, step/2.0f);
			grid.p[7] = pos+vec3(-step/2.0f, step/2.0f, step/2.0f);


			int r = ipow(2, recurse);
			
			if      (bottom[ r][-r].Set) { grid.val[0] = bottom[ r][-r].Dist; } 
			else if (left  [-r][-r].Set) { grid.val[0] = left  [-r][-r].Dist; } 
			else if (back  [-r][-r].Set) { grid.val[0] = back  [-r][-r].Dist; }
			else { grid.val[0] = SignedDistance(grid.p[0]); bottom[ r][-r] = Distance{grid.val[0], true}; left [-r][-r] = Distance{grid.val[0], true}; back [-r][-r] = Distance{grid.val[0], true}; }

			if      (bottom[ r][ r].Set) { grid.val[1] = bottom[ r][ r].Dist; } 
			else if (right [-r][-r].Set) { grid.val[1] = right [-r][-r].Dist; } 
			else if (back  [-r][ r].Set) { grid.val[1] = back  [-r][ r].Dist; }
			else { grid.val[1] = SignedDistance(grid.p[1]); bottom[ r][ r] = Distance{grid.val[1], true}; right[-r][-r] = Distance{grid.val[1], true}; back[-r][ r] = Distance{grid.val[1], true}; }

			if      (bottom[-r][ r].Set) { grid.val[2] = bottom[-r][ r].Dist; } 
			else if (right [-r][ r].Set) { grid.val[2] = right [-r][ r].Dist; } 
			else if (front [-r][ r].Set) { grid.val[2] = front [-r][ r].Dist; }
			else { grid.val[2] = SignedDistance(grid.p[2]); bottom[-r][ r] = Distance{grid.val[2], true}; right[-r][ r] = Distance{grid.val[2], true}; front[-r][ r] = Distance{grid.val[2], true}; }

			if      (bottom[-r][-r].Set) { grid.val[3] = bottom[-r][-r].Dist; } 
			else if (left  [-r][ r].Set) { grid.val[3] = left  [-r][ r].Dist; } 
			else if (front [-r][-r].Set) { grid.val[3] = front [-r][-r].Dist; } 
			else { grid.val[3] = SignedDistance(grid.p[3]); bottom[-r][-r] = Distance{grid.val[3], true}; left [-r][ r] = Distance{grid.val[3], true}; front[-r][-r] = Distance{grid.val[3], true}; }


			if      (top [ r][-r].Set) { grid.val[4] = top [ r][-r].Dist; } 
			else if (left[ r][-r].Set) { grid.val[4] = left[ r][-r].Dist; } 
			else if (back[ r][-r].Set) { grid.val[4] = back[ r][-r].Dist; }
			else { grid.val[4] = SignedDistance(grid.p[4]); top[ r][-r] = Distance{grid.val[4], true}; left[ r][-r] = Distance{grid.val[4], true}; back[ r][-r] = Distance{grid.val[4], true}; }
			
			if      (top  [ r][ r].Set) { grid.val[5] = top  [ r][ r].Dist; } 
			else if (right[ r][-r].Set) { grid.val[5] = right[ r][-r].Dist; } 
			else if (back [ r][ r].Set) { grid.val[5] = back [ r][ r].Dist; }
			else { grid.val[5] = SignedDistance(grid.p[5]); top[ r][ r] = Distance{grid.val[5], true}; right[ r][-r] = Distance{grid.val[5], true}; back[ r][ r] = Distance{grid.val[5], true}; }

			if      (top  [-r][ r].Set) { grid.val[6] = top  [-r][ r].Dist; } 
			else if (right[ r][ r].Set) { grid.val[6] = right[ r][ r].Dist; } 
			else if (front[ r][ r].Set) { grid.val[6] = front[ r][ r].Dist; }
			else { grid.val[6] = SignedDistance(grid.p[6]); top[-r][ r] = Distance{grid.val[6], true}; right[ r][ r] = Distance{grid.val[6], true}; front[ r][ r] = Distance{grid.val[6], true}; }
			
			if      (top  [-r][-r].Set) { grid.val[7] = top  [-r][-r].Dist; } 
			else if (left [ r][ r].Set) { grid.val[7] = left [ r][ r].Dist; } 
			else if (front[ r][-r].Set) { grid.val[7] = front[ r][-r].Dist; } 
			else { grid.val[7] = SignedDistance(grid.p[7]); top[-r][-r] = Distance{grid.val[7], true}; left [ r][ r] = Distance{grid.val[7], true}; front[ r][-r] = Distance{grid.val[7], true}; }


			int numtri = Polygonise(grid, 0.0, triangles);

			for (int i=0; i<numtri; i++)
			{
				VertexData[NumEntries++] = triangles[i].p[0].x;
				VertexData[NumEntries++] = triangles[i].p[0].y;
				VertexData[NumEntries++] = triangles[i].p[0].z;

				VertexData[NumEntries++] = triangles[i].p[1].x;
				VertexData[NumEntries++] = triangles[i].p[1].y;
				VertexData[NumEntries++] = triangles[i].p[1].z;

				VertexData[NumEntries++] = triangles[i].p[2].x;
				VertexData[NumEntries++] = triangles[i].p[2].y;
				VertexData[NumEntries++] = triangles[i].p[2].z;

				if (NumEntries+9 >= VertDataSize)
				{
					VertDataSize += BUFF_STEP_SIZE;
					std::cout << "Realloc: " << sizeof(float)*VertDataSize/1000/1000 << "MB\n";
					VertexData = (float *)realloc(VertexData, sizeof(float)*VertDataSize);
				}
			}
		}
	}
}


int pass = 0;
//Faster than RecursiveMarch3 when SignedDistance is fast to compute (aka C++ implementation)
void RecursiveMarch4(vec3 xyz, float step, int recurse,
	float dist1, float dist2, float dist3, float dist4, float dist5, float dist6, float dist7, float dist8,
	bool d1En, bool d2En, bool d3En, bool d4En, bool d5En, bool d6En, bool d7En, bool d8En)
{
	float dist = SignedDistance(xyz);

	//abs(dist) <= sqrt(3)
	if (abs(dist) <= step*1.735f)//*2.0f) //sqrt(3.0f))
	{
		if (recurse)
		{
			RecursiveMarch4(xyz+vec3(-step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, dist1, 0,     0,     0,      0,    0,    dist, 0,        d1En,  false, false, false,  false, false, true,  false);
			RecursiveMarch4(xyz+vec3( step/4.0f,-step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,     dist2, 0,     0,      0,    0,    0,    dist,     false, d2En,  false, false,  false, false, false, true );
			RecursiveMarch4(xyz+vec3( step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,     0,     dist3, 0,      dist, 0,    0,    0,        false, false, d3En,  false,  true,  false, false, false);
			RecursiveMarch4(xyz+vec3(-step/4.0f,-step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,     0,     0,     dist4,  0,    dist, 0,    0,        false, false, false, d4En,   false, true,  false, false);

			RecursiveMarch4(xyz+vec3(-step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,    0,    dist, 0,         dist5, 0,     0,     0,     false, false, true,  false,  d5En,  false, false, false);
			RecursiveMarch4(xyz+vec3( step/4.0f, step/4.0f,-step/4.0f), step/2.0f, recurse-1, 0,    0,    0,    dist,      0,     dist6, 0,     0,     false, false, false, true,   false, d6En,  false, false);
			RecursiveMarch4(xyz+vec3( step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, dist, 0,    0,    0,         0,     0,     dist7, 0,     true,  false, false, false,  false, false, d7En,  false);
			RecursiveMarch4(xyz+vec3(-step/4.0f, step/4.0f, step/4.0f), step/2.0f, recurse-1, 0,    dist, 0,    0,         0,     0,     0,     dist8, false, true,  false, false,  false, false, false, d8En );
		}
		else
		{
			MarchingCubes(xyz, step, 80);
		}
	}
	else { pass++; }
}

extern "C"
{

float *MarchingCubes(unsigned int *numEntries)
{
	VertDataSize = BUFF_STEP_SIZE;
	VertexData = (float *)malloc(sizeof(float)*VertDataSize);
	NumEntries=0;

	struct timespec t1 = GetTime();
	// MarchingCubes(vec3(0.0f), 1.6f, 155);
	// RecursiveMarch(vec3(0.0f), 10.0f, 10);
	// RecursiveMarch2(vec3(0.0f), 10.0f, 10, 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, false, false, false, false, false);
	int recurse = 7;
	int sidelen = ipow(2, recurse+1)+1;
	RecursiveMarch3(vec3(0.0f), 1.6f, recurse, Array2D(sidelen), Array2D(sidelen), Array2D(sidelen), Array2D(sidelen), Array2D(sidelen), Array2D(sidelen));
	// RecursiveMarch4(vec3(0.0f), 1.6f, 3, 0, 0, 0, 0, 0, 0, 0, 0, false, false, false, false, false, false, false, false);
	// BatchingMarch(1.6f, 10, 80);
	struct timespec t2 = GetTime();

	printf("Marching Cubes Calculation Time: ");
	PrintDuration(t1, t2);
	printf("\n");

	*numEntries = NumEntries;
	return VertexData;
}

}
