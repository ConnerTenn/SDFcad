
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS = -lm
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

make = $(MAKE) --no-print-directory -j8

.PHONY: all clean SignedDistance.so MeshExporter


all: clean
	#Generate library with -fPIC so that it can be used in a dynamic library later
	$(make) SDFlib.a CXXFLAGS="$(CXXFLAGS) -fPIC"
	#Generate SDFcad without -fPIC
	$(make) SDFcad
	$(make) MeshExporter

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o SDFlib.a Common.o shader.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

MeshExporter: MeshExporter.o Common.o SDFlib.a
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#Library with all the Signed Distance Framework
SDFlib.a: SDFlib.o MarchingCubes.o StlWriter.o SignedDistanceHelper.o
	ar rvs $@ $^

#Generate the dynamic library for the Signed Distance function
SignedDistance.so: SDFlib.o
	#Use of  -Wl,--whole-archive [lib] -Wl,--no-whole-archive  to make sure all symbols from the static library is included in the new dynamic library
	$(CXX) -shared -fPIC $(CXXFLAGS) $^ $(SDF_FILE) -o $@


clean :
	rm -f SDFcad *.o *.a *.so

