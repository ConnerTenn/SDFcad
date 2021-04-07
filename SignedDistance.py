
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


def sdUnion(d0, d1):
	return min(d0, d1)

def sdInter(d0, d1):
	return max(d0, d1)

@ffi.callback('float(float, float, float)')
def SignedDistance(x, y, z):
	# print(x,y,z)
	pos = Vec3(x,y,z)
	# print(pos)

	p1 = translate(pos, Vec3(0.6,0,0))
	p2 = translate(pos, Vec3(-0.6,0,0))

	d0 = sdBox(p1, Vec3(0.5,0.5,0.5))
	d1 = sdSphere(p1, 0.6)
	d2 = sdSphere(p2, 0.6)
	
	d = sdInter(d1, d0) + \
		sin(p1.Y*2*pi*10.0)/50.0 + \
		sin(p1.X*2*pi*10.0)/100.0 + \
		sin(p1.Z*2*pi*10.0)/100.0

	# print(d)
	return sdUnion(d, d2)


# ffi.cdef('void callback(float (*func)(float, float, float));')
c_func = ffi.cast('float(*)(float(*)(float, float, float))', c_argument)
c_func(SignedDistance)
