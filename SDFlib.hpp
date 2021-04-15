
#ifndef _SDF_LIB_H_
#define _SDF_LIB_H_

#include "Common.hpp"


vec3 translate(vec3 pos, vec3 move);
float sdSphere(vec3 pos, float r);
float sdBox(vec3 p, vec3 b);
float sdBoxFast(vec3 pos, vec3 l);
float sdCappedCylinder(vec3 pos, float h, float r);

float sdUnion(float d0, float d1);
float sdInter(float d0, float d1);

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

class SDF3;

//== Objects ==
SDF3 Sphere(float radius);
SDF3 SphereD(float diameter);
SDF3 Box(vec3 size);
SDF3 Cube(float size);
SDF3 Cylinder(float height, float radius);

//== Modifiers ==
SDF3 Translate(SDF3 object, vec3 move);
SDF3 Union(SDF3 object1, SDF3 object2);
SDF3 Difference(SDF3 object1, SDF3 object2);
SDF3 Intersect(SDF3 object1, SDF3 object2);
SDF3 Transform(SDF3 object, mat4 matrix);
SDF3 Rotate(SDF3 object, float angle, vec3 axis);


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


	float operator()(vec3 pos)
	{
		return (*Object)(pos);
	}
};


//=====================
//==     Objects     ==
//=====================

class SDSphere : public _SDF3
{
	float Radius;
public:
	SDSphere(float radius) : _SDF3()
	{
		// std::cout << "+ SDSphere\n";
		Radius = radius;
	}
	~SDSphere() {} //{ std::cout << "- SDSphere\n"; }


	_SDF3 *Duplicate() const
	{
		return new SDSphere(Radius);
	}

	float operator()(vec3 pos)
	{
		return length(pos) - Radius;
	}
};

class SDBox : public _SDF3
{
	vec3 Size;
public:
	SDBox(vec3 size) : _SDF3()
	{
		// std::cout << "+ SDBox\n";
		Size = size/2.0f;
	}
	~SDBox() {} //{ std::cout << "- SDBox\n"; }


	_SDF3 *Duplicate() const
	{
		return new SDBox(Size);
	}

	float operator()(vec3 pos)
	{
		vec3 b = abs(pos) - Size;
		return max(max(b.x, b.y), b.z);
	}
};

class SDCylinder : public _SDF3
{
	float Height;
	float Radius;
public:
	SDCylinder(float height, float radius) : _SDF3()
	{
		Height = height;
		Radius = radius;
	}
	~SDCylinder() {}


	_SDF3 *Duplicate() const
	{
		return new SDCylinder(Height, Radius);
	}

	float operator()(vec3 pos)
	{
		float l = sqrt(pow(pos.x,2.0f) + pow(pos.z,2.0f));
		float dx = abs(l) - Radius;
		float dy = abs(pos.y) - Height;
		return min(max(dx,dy),0.0f) + sqrt(pow(max(dx,0.0f),2.0f) + pow(max(dy,0.0f),2.0f));
	}
};


//=====================
//==    Modifiers    ==
//=====================

class SDTranslate : public _SDF3
{
	SDF3 Object;
	vec3 Move;
public:
	SDTranslate(SDF3 object, vec3 move) : _SDF3()
	{
		// std::cout << "+ SDTranslate\n";
		Object = object;
		Move = move;
	}
	~SDTranslate() {} //{ std::cout << "- SDTranslate\n"; }

	_SDF3 *Duplicate() const
	{
		return new SDTranslate(Object, Move);
	}

	float operator()(vec3 pos)
	{
		// std::cout << "Translate()\n";
		return (*Object.Object)(pos - Move);
	}
};

class SDUnion : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDUnion(SDF3 object1, SDF3 object2) : _SDF3()
	{
		// std::cout << "+ SDUnion\n";
		Object1 = object1;
		Object2 = object2;
	}
	~SDUnion() {} //{ std::cout << "- SDUnion\n"; }

	_SDF3 *Duplicate() const
	{
		return new SDUnion(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return min((*Object1.Object)(pos), (*Object2.Object)(pos));
	}
};

class SDDifference : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDDifference(SDF3 object1, SDF3 object2) : _SDF3()
	{
		// std::cout << "+ SDDifference\n";
		Object1 = object1;
		Object2 = object2;
	}
	~SDDifference() {} //{ std::cout << "- SDDifference\n"; }

	_SDF3 *Duplicate() const
	{
		return new SDDifference(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return max((*Object1.Object)(pos), -(*Object2.Object)(pos));
	}
};

class SDIntersect : public _SDF3
{
	SDF3 Object1;
	SDF3 Object2;
public:
	SDIntersect(SDF3 object1, SDF3 object2) : _SDF3()
	{
		// std::cout << "+ SDIntersect\n";
		Object1 = object1;
		Object2 = object2;
	}
	~SDIntersect() {} //{ std::cout << "- SDIntersect\n"; }

	_SDF3 *Duplicate() const
	{
		return new SDIntersect(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return max((*Object1.Object)(pos), (*Object2.Object)(pos));
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
		vec4 res = TransformMat * vec4(pos.x, pos.y, pos.z, 1);
		return (*Object.Object)(vec3(res.x, res.y, res.z));
	}
};



extern "C"
{
// == User Defined ==
void ConstructSignedDistance();
void DestructSignedDistance();
float SignedDistance(vec3 pos);
// == End User Defined ==

//Exported interface to the signed distance marching cubes implementation
float *MarchingCubes(unsigned int *numEntries);

}

#endif
