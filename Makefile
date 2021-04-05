
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS = -lm
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

make = $(MAKE) --no-print-directory -j8

all: clean
	$(make) RayMarch.fragshader
	$(make) $(TARGET)

%.o: %.c
	$(C) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o Common.o shader.o MarchingCubes.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

RayMarch.fragshader:
	cpp -P RayMarch.frag -o $@

.PHONY : clean
clean :
	rm -f SDFcad RayMarch.fragshader *.o

