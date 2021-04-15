
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
protected:
public:
	//Used for reference counting so object can be deleted when no longer in use.

	int ReferenceCount = 0;
	_SDF3()
	{
	}
	virtual ~_SDF3()
	{
	}

	virtual _SDF3 *Duplicate() const = 0;

	virtual float operator()(vec3 pos) = 0;
};


class SDF3
{
public:
	_SDF3 *Object = 0;

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

	SDF3()
	{
	}
	SDF3(_SDF3 *obj)
	{
		ObjectDecr();
		Object = obj;//->Duplicate();
		Object->ReferenceCount++;
	}
	SDF3(SDF3 &other)
	{
		// std::cout << "SDF3 Ref Copy\n";
		ObjectDecr();
		Object = other.Object->Duplicate();
		Object->ReferenceCount++;
	}
	SDF3(const SDF3 &other)
	{
		// std::cout << "SDF3 Const Ref Copy\n";
		ObjectDecr();
		Object = other.Object->Duplicate();
	}
	void operator=(SDF3 &other)
	{
		// std::cout << "SDF3 Assign\n";
		ObjectDecr();
		Object = other.Object->Duplicate();
		Object->ReferenceCount++;
	}
	void operator=(const SDF3 &other)
	{
		// std::cout << "SDF3 Const Assign\n";
		ObjectDecr();
		Object = other.Object->Duplicate();
		Object->ReferenceCount++;
	}
	~SDF3()
	{
		ObjectDecr();
	}


	float operator()(vec3 pos)
	{
		return (*Object)(pos);
	}
};

class SDSphere : public _SDF3
{
	float Radius;
public:
	SDSphere(float radius) : _SDF3()
	{
		// std::cout << "Create Sphere " << radius <<"\n";
		Radius = radius;
	}
	~SDSphere()
	{
		// std::cout << "Destroy Sphere " << Radius <<"\n";
	}


	_SDF3 *Duplicate() const
	{
		return new SDSphere(Radius);
	}

	float operator()(vec3 pos)
	{
		// std::cout << "Sphere()\n";
		return length(pos) - Radius;
	}
};

class SDTranslate : public _SDF3
{
	SDF3 Object;
	vec3 Move;
public:
	SDTranslate(SDF3 object, vec3 move) : _SDF3()
	{
		// std::cout << "Create Translate " <<"\n";
		Object = object;
		Move = move;
	}
	~SDTranslate()
	{
		// std::cout << "Destroy Translate " <<"\n";
	}

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
		// std::cout << "Create Union " <<"\n";
		Object1 = object1;
		Object2 = object2;
	}
	~SDUnion()
	{
		// std::cout << "Destroy Union " <<"\n";
	}

	_SDF3 *Duplicate() const
	{
		return new SDUnion(Object1, Object2);
	}

	float operator()(vec3 pos)
	{
		return min((*Object1.Object)(pos), (*Object2.Object)(pos));
	}
};


SDF3 Sphere(float radius);
SDF3 Translate(SDF3 object, vec3 move);
SDF3 Union(SDF3 object1, SDF3 object2);


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
