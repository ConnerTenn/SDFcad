
CC = gcc
CXX = g++

#Generate library with -fPIC so that it can be used in a dynamic library later
CFLAGS = -Wall -fPIC -O3
CXXFLAGS = -Wall -fPIC -O3
LDFLAGS =

make = $(MAKE) --no-print-directory -j8

.PHONY: all clean SDFlib.a SignedDistance.so MeshExporter


all: clean
	$(make) SDFlib.a

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

#Library with all the Signed Distance Framework
SDFlib.a: SDFlib.o Common.o MarchingCubes.o StlWriter.o SignedDistanceHelper.o
	ar rvs $@ $^

#Generate the dynamic library for the Signed Distance function
SignedDistance.so: SDFlib.o
	$(CXX) -shared $(CXXFLAGS) $^ $(SDF_FILE) -o $@


clean :
	rm -f *.o *.a *.so

