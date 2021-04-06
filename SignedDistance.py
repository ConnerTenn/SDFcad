
from math import *
import numpy as np

def vec3(x,y,z):
	return np.array([[x],[y],[z]])

def length(vec):
	return np.linalg.norm(vec)

def translate(pos, move):
	return pos - move

def sdSphere(pos, r):
	return length(pos) - r

def sdBox(p, b):
	d = abs(p) - b
	return min(max(d[0],max(d[1],d[2])),0.0) + length(np.maximum(d,0.0))


def sdUnion(d0, d1):
	return min(d0, d1)

def sdInter(d0, d1):
	return max(d0, d1)

def SignedDistance(x,y,z):
	# print(x,y,z)
	pos = vec3(x,y,z)
	# print(pos)

	p1 = translate(pos, vec3(0.6,0,0))
	p2 = translate(pos, vec3(-0.6,0,0))

	d0 = sdBox(p1, vec3(0.5,0.5,0.5))
	d1 = sdSphere(p1, 0.6)
	d2 = sdSphere(p2, 0.6)
	
	d = sdInter(d1, d0) + \
		sin(p1[0]*2*pi*10.0)/100.0 + \
		sin(p1[1]*2*pi*10.0)/500.0 + \
		sin(p1[2]*2*pi*10.0)/500.0

	# print(d)
	return sdUnion(d, d2)

