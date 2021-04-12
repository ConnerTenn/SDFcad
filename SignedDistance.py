
from cffi import FFI
ffi = FFI()

from math import *

class Vec3:
	def __init__(self, x, y, z):
		self.X = x
		self.Y = y
		self.Z = z
	
	def __add__(self, vec):
		return Vec3(self.X + vec.X, self.Y + vec.Y, self.Z + vec.Z)
	def __sub__(self, vec):
		return Vec3(self.X - vec.X, self.Y - vec.Y, self.Z - vec.Z)

	def __abs__(self):
		return Vec3(abs(self.X), abs(self.Y), abs(self.Z))

	def max(self, s):
		return Vec3(max(self.X, s), max(self.Y, s), max(self.Z, s))

def length(vec):
	return sqrt(vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z)

def translate(pos, move):
	return pos - move

def sdSphere(pos, r):
	return length(pos) - r

def sdBox(p, b):
	d = abs(p) - b
	return min(max(d.X,max(d.Y,d.Z)),0.0) + length(d.max(0))

def sdBoxFast(pos, l):
	b = abs(pos) - l
	return max(max(b.X, b.Y), b.Z)


def sdUnion(d0, d1):
	return min(d0, d1)

def sdInter(d0, d1):
	return max(d0, d1)

def sdCappedCylinder(pos, h, r):
	l = sqrt(pos.X**2 + pos.Z**2)
	dx = abs(l) - r
	dy = abs(pos.Y) - h
	return min(max(dx,dy),0.0) + sqrt(max(dx,0)**2 + max(dy,0)**2)

@ffi.callback('float(float, float, float)')
def SignedDistance(x, y, z):
	pos = Vec3(x,y,z)

	f = max(sdSphere(pos, 1), sdBoxFast(pos, Vec3(1.5/2,1.5/2,1.5/2)))

	posz = Vec3(pos.X, pos.Z, pos.Y)
	posx = Vec3(pos.Y, pos.X, pos.Z)
	t = min([sdCappedCylinder(posx, 2, 1/2), sdCappedCylinder(pos, 2, 1/2), sdCappedCylinder(posz, 2, 1/2) ])

	return max(f, -t)


# ffi.cdef('void callback(float (*func)(float, float, float));')
c_func = ffi.cast('float(*)(float(*)(float, float, float))', c_argument)
c_func(SignedDistance)
