
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3 -I/usr/include/python3.9 #$(shell python3.9-config --cflags)
LDFLAGS = -lm -ldl
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)
LDFLAGS += -lpython3.9 #$(shell python3.9-config --ldflags)

make = $(MAKE) --no-print-directory -j8

all: clean
	$(make) RayMarch.fragshader
	$(make) PySignedDistance.so
	$(make) SDFcad

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o Common.o shader.o MarchingCubes.o SignedDistance.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

RayMarch.fragshader:
	cpp -P RayMarch.frag -o $@

PySignedDistance.c:
	cython -3 PySignedDistance.pyx
PySignedDistance.so: PySignedDistance.c
	gcc -shared -fPIC -I/usr/include/python3.9 $^ -o $@

.PHONY : clean
clean :
	rm -f SDFcad RayMarch.fragshader PySignedDistance.c PySignedDistance.h PySignedDistance.so *.o

