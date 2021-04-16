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

# SDFviewer

A simple Mesh Viewer for the SDFcad implementation.


# MeshExporter

A command line for generating the mesh and exporting to an .stl file.


[Inigo Quilez]: https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

