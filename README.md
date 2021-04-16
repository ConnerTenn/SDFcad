# SDFcad

This is a pretty early work in progress! There's still a lot of work to be done, features to be added, and things to clean up. I'll be refining it over time.

Special thanks to [Sebastian Lague's Coding Adventure](https://youtu.be/Cp5WWtMoeKg) and [Michael Fogleman](https://github.com/fogleman/sdf/) for a lot of the inspiration for this program. Also thanks to [Inigo Quilez] for documentating the implementation for a large variety of signed distance functions.

# Example

<img width=350 src="Images/Cover.png">

Similar to [Inigo Quilez], here's the same example implemented with this SDF library.

```C++

SDF3 Object;

void ConstructSignedDistance()
{
    SDF3 cylinder = Cylinder(2, 0.5);
    SDF3 cylinderX = Rotate(cylinder, TO_RAD(90), vec3(1,0,0));
    SDF3 cylinderZ = Rotate(cylinder, TO_RAD(90), vec3(0,0,1)); 

    Object = (Sphere(1) & Cube(1.5)) - (cylinder + cylinderX + cylinderZ);
}

void DestructSignedDistance()
{
}

float SignedDistance(vec3 pos)
{
    return Object(pos);
}

```

# How it works

Objects are first initialized in the function ConstructSignedDistance. This is so that the initialization only needs to happen once and makes the calls to SignedDistance faster.

SDF objects can be called with vec3 as a parameter to evaluate the signed distance function at that position. In this example, all that needs to be done is to call Object with the position argument.

## Operations

Operator(s) | Action
-|-
a+b <br> a\|b | Union of two objects. Encapsulates the Union() function.
a-b | Difference between two objects. Encapsulates the Difference() function.
a&b | Intersection between two objects. Encapsulates the Intersection() function.

## Under the hood

SDF3 objects are just containers for the underlying _SDF objects. They handle the overloaded operators and memory management. 

Functions are used to abstract away and simplify the object creation. All objects like spheres and boxes inherit from the _SDF class. This allows them to be referenced polymorphically from the SDF object containing it.

Each time an object is created, a new _SDF object is allocated and contained within an SDF object. _SDF objects are passed around by reference which minimizes the amount of times memory has to be allocated and freed. To enable this, each _SDF one keeps track of their reference count. When a SDF object obtains an _SDF object, it increments the reference counter. When it is destroyed, it decrements the reference counter. If the reference counter of an _SDF object reaches zero, it is freed.

<br>

From the above example, a new SDFCylinder object is allocated and stored within the cylinder variable. The Rotate function creates a SDFRotate object which stores a reference to the original cylinder, along with the transformation matrix. Therefore the creation of the cylinderX and cylinderZ objects only allocate 2 new SDFRotate objects, with the original cylinder being referenced in 3 places.

The final statement creates a new SDFSphere and SDFBox object, as well as a SDFIntersection object to operate on them. SDFDifference and SDFUnion objects are also then created for the following operations.

# SDFviewer

A simple Mesh Viewer for the SDFcad implementation.


# MeshExporter

A command line for generating the mesh and exporting to an .stl file.


[Inigo Quilez]: https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

