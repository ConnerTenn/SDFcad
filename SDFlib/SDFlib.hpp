
#ifndef _SDF_LIB_H_
#define _SDF_LIB_H_

#include "../Common.hpp"


//=====================
//==     Objects     ==
//=====================

inline float SdfSphere(vec3 pos, float radius)
{
	return length(pos) - radius;
}
inline float SdfRoundBox(vec3 pos, vec3 size, float radius)
{
	vec3 d = abs(pos) - size;
	return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d, vec3(0.0f))) - radius;
}
inline float SdfBox(vec3 pos, vec3 size)
{
	vec3 b = abs(pos) - size;
	return max(max(b.x, b.y), b.z);
}
inline float SdfCylinder(vec3 pos, float height, float radius)
{
		float l = sqrt(pow(pos.x,2.0f) + pow(pos.z,2.0f));
		float dx = abs(l) - radius;
		float dy = abs(pos.y) - height;
		return min(max(dx,dy),0.0f) + sqrt(pow(max(dx,0.0f),2.0f) + pow(max(dy,0.0f),2.0f));
}


//=====================
//==    Modifiers    ==
//=====================

inline float SdfUnion(float d0, float d1)
{
	return min(d0, d1);
}
inline float SdfDifference(float d0, float d1)
{
	return max(d0, -d1);
}
inline float SdfIntersect(float d0, float d1)
{
	return max(d0, d1);
}
inline vec3 SdfTranslate(vec3 pos, vec3 move)
{
	return pos - move;
}
inline vec3 SdfTransform(vec3 pos, mat4 matrix)
{
	vec4 res = matrix * vec4(pos.x, pos.y, pos.z, 1);
	return vec3(res.x, res.y, res.z);
}



//===================================
//==    SDF Objects & Functions    ==
//===================================

class SDF3;

//== Object Wrappers ==
SDF3 Sphere(float radius);
SDF3 SphereD(float diameter);
SDF3 Box(vec3 size);
SDF3 Cube(float size);
SDF3 Cylinder(float height, float radius);

//== Modifier Wrappers ==
SDF3 Translate(SDF3 object, vec3 move);
SDF3 Union(SDF3 object1, SDF3 object2);
SDF3 Difference(SDF3 object1, SDF3 object2);
SDF3 Intersect(SDF3 object1, SDF3 object2);
SDF3 Transform(SDF3 object, mat4 matrix);
SDF3 Rotate(SDF3 object, float angle, vec3 axis);

//Virtual parent class. Used for polymorphic access to all SDF3 objects
class _SDF3
{
public:
	//Used for reference counting so object can be deleted when no longer in use.
	int ReferenceCount = 0;

	_SDF3() {}
	virtual ~_SDF3() {}

	virtual _SDF3 *Duplicate() const = 0;

	virtual float operator()(vec3 pos) = 0;
};

//SDF3 container class for use in user code. Handles memory management of _SDF3 objects.
class SDF3
{
public:
	_SDF3 *Object = 0;

	//Handles reference tracking and automatic freeing
	void ObjectDecr()
	{
		if (Object)
		{
			Object->ReferenceCount--;
			if (Object->ReferenceCount==0)
			{
				delete Object;
			}
		}
		Object = 0;
	}

	SDF3() { }

	SDF3(_SDF3 *obj)
	{
		ObjectDecr();
		Object = obj;
		Object->ReferenceCount++;
	}
	SDF3(SDF3 &other)
	{
		// std::cout << "SDF3 Ref Copy\n";
		ObjectDecr();
		Object = other.Object;//->Duplicate();
		Object->ReferenceCount++;
	}
	SDF3(const SDF3 &other)
	{
		// std::cout << "SDF3 Const Ref Copy\n";
		ObjectDecr();
		Object = other.Object;//->Duplicate();
		Object->ReferenceCount++;
	}
	void operator=(SDF3 &other)
	{
		// std::cout << "SDF3 Assign\n";
		ObjectDecr();
		Object = other.Object;//->Duplicate();
		Object->ReferenceCount++;
	}
	void operator=(const SDF3 &other)
	{
		// std::cout << "SDF3 Const Assign\n";
		ObjectDecr();
		Object = other.Object;//->Duplicate();
		Object->ReferenceCount++;
	}
	~SDF3()
	{
		ObjectDecr();
	}

	SDF3 Duplicate()
	{
		return SDF3(Object->Duplicate());
	}


	SDF3 operator+(SDF3 other)
	{
		return Union(*this, other);
	}
	SDF3 operator|(SDF3 other)
	{
		return Union(*this, other);
	}
	SDF3 operator-(SDF3 other)
	{
		return Difference(*this, other);
	}
	SDF3 operator&(SDF3 other)
	{
		return Intersect(*this, other);
	}

	//Evaluates the Signed Distance Function for a given point
	float operator()(vec3 pos)
	{
		return (*Object)(pos);
	}
};


//=====================
//==     Objects     ==
//=====================

class SDFSphere : public _SDF3
{
	float Radius;
public:
	SDFSphere(float radius) : _SDF3()
	{
		Radius = radius;
	}
	~SDFSphere() {}


	_SDF3 *Duplicate() const
	{
		return new SDFSphere(Radius);
	}

	float operator()(vec3 pos)
	{
		return SdfSphere(pos, Radius);
	}
};

class SDFBox : public _SDF3
{
	vec3 Size;
public:
	SDFBox(vec3 size) : _SDF3()
	{
		Size = size/2.0f;
	}
	~SDFBox() {}


	_SDF3 *Duplicate() const
	{
		return new SDFBox(Size);
	}

	float operator()(vec3 pos)
	{
		return SdfBox(pos, Size);
	}
};

class SDFCylinder : public _SDF3
{
	float Height;
	float Radius;
public:
	SDFCylinder(float height, float radius) : _SDF3()
	{
		Height = height;
		Radius = radius;
	}
	~SDFCylinder() {}


	_SDF3 *Duplicate() const
	{
		return new SDFCylinder(Height, Radius);
	}

	float operator()(vec3 pos)
	{
		return SdfCylinder(pos, Height, Radius);
	}
};


//=====================
//==    Modifiers    ==
//=====================

class SDFUnion : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDFUnion(SDF3 object1, SDF3 object2) : _SDF3()
	{
		Object1 = object1;
		Object2 = object2;
	}
	~SDFUnion() {}

	_SDF3 *Duplicate() const
	{
		return new SDFUnion(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return SdfUnion((*Object1.Object)(pos), (*Object2.Object)(pos));
	}
};

class SDFDifference : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDFDifference(SDF3 object1, SDF3 object2) : _SDF3()
	{
		Object1 = object1;
		Object2 = object2;
	}
	~SDFDifference() {}

	_SDF3 *Duplicate() const
	{
		return new SDFDifference(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return SdfDifference((*Object1.Object)(pos), (*Object2.Object)(pos));
	}
};

class SDFIntersect : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDFIntersect(SDF3 object1, SDF3 object2) : _SDF3()
	{
		Object1 = object1;
		Object2 = object2;
	}
	~SDFIntersect() {}

	_SDF3 *Duplicate() const
	{
		return new SDFIntersect(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return SdfIntersect((*Object1.Object)(pos), (*Object2.Object)(pos));
	}
};

class SDFTranslate : public _SDF3
{
	SDF3 Object;
	vec3 Move;
public:
	SDFTranslate(SDF3 object, vec3 move) : _SDF3()
	{
		Object = object;
		Move = move;
	}
	~SDFTranslate() {}

	_SDF3 *Duplicate() const
	{
		return new SDFTranslate(Object, Move);
	}

	float operator()(vec3 pos)
	{
		return (*Object.Object)(SdfTranslate(pos, Move));
	}
};

class SDTransform : public _SDF3
{
	SDF3 Object;
	mat4 TransformMat;
public:
	SDTransform(SDF3 object, mat4 matrix) : _SDF3()
	{
		Object = object;
		TransformMat = matrix;

	}
	~SDTransform() {}

	_SDF3 *Duplicate() const
	{
		return new SDTransform(Object, TransformMat);
	}

	float operator()(vec3 pos)
	{
		return (*Object.Object)(SdfTransform(pos, TransformMat));
	}
};



//============================
//==    Helper Functions    ==
//============================

vec3 CalculateNormal(vec3 p1, vec3 p2, vec3 p3);

float *GenerateMesh(unsigned int *numEntries, float sidelen, float minsize);


//================
//==    User    ==
//================

extern "C"
{
	
// == User Defined ==
void ConstructSignedDistance();
void DestructSignedDistance();
float SignedDistance(vec3 pos);
// == End User Defined ==


}

extern float (*UserSignedDistance)(vec3);

#endif
