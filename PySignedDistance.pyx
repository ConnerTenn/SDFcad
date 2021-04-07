
from math import *

cdef class Vec3:
	cdef public float X
	cdef public float Y
	cdef public float Z
	def __init__(self, x, y, z):
		self.X = x
		self.Y = y
		self.Z = z
	
	def __add__(Vec3 self, Vec3 vec):
		return <Vec3>Vec3(self.X + vec.X, self.Y + vec.Y, self.Z + vec.Z)
	def __sub__(Vec3 self, Vec3 vec):
		return <Vec3>Vec3(self.X - vec.X, self.Y - vec.Y, self.Z - vec.Z)

	def __abs__(Vec3 self):
		return <Vec3>Vec3(<float>abs(self.X), <float>abs(self.Y), <float>abs(self.Z))

	def max(Vec3 self, float s):
		return <Vec3>Vec3(<float>max(self.X, s), <float>max(self.Y, s), <float>max(self.Z, s))

cdef float length(Vec3 vec):
	return <float>sqrt(vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z)

cdef Vec3 translate(Vec3 pos, Vec3 move):
	return pos - move

cdef float sdSphere(Vec3 pos, float r):
	return <float>length(pos) - r

cdef float sdBox(Vec3 p, Vec3 b):
	d = <Vec3>abs(p) - b
	return <float>min(<float>max(d.X,<float>max(d.Y,d.Z)),0.0) + <float>length(<Vec3>d.max(0))


cdef float sdUnion(float d0, float d1):
	return <float>min(d0, d1)

cdef float sdInter(float d0, float d1):
	return <float>max(d0, d1)

cdef public float SignedDistance(float x, float y, float z):
	# print(x,y,z)
	cdef Vec3 pos = <Vec3>Vec3(x,y,z)
	# print(pos)

	cdef Vec3 p1 = <Vec3>translate(pos, <Vec3>Vec3(0.6,0,0))
	cdef Vec3 p2 = <Vec3>translate(pos, <Vec3>Vec3(-0.6,0,0))

	cdef float d0 = sdBox(p1, <Vec3>Vec3(0.5,0.5,0.5))
	cdef float d1 = sdSphere(p1, 0.6)
	cdef float d2 = sdSphere(p2, 0.6)
	
	cdef float d = sdInter(d1, d0) + \
		<float>sin(p1.Y*2*pi*10.0)/50.0 + \
		<float>sin(p1.X*2*pi*10.0)/100.0 + \
		<float>sin(p1.Z*2*pi*10.0)/100.0

	# print(d)
	return <float>sdUnion(d, d2)

