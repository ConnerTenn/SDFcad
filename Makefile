
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS =
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

make = $(MAKE) --no-print-directory -j8

.PHONY: all clean SDFcad MeshExporter


all: clean 
	$(make) SDFcad
	$(make) MeshExporter

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o SDFlib/SDFlib.a shader.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

MeshExporter: MeshExporter.o SDFlib/SDFlib.a
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#Call recursive make for the Signed Distance library
SDFlib/SDFlib.a:
	$(make) -C SDFlib/ SDFlib.a


clean :
	rm -f SDFcad *.o *.a *.so
	$(make) -C SDFlib/ clean

