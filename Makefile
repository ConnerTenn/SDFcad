
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS = -lm
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

make = $(MAKE) --no-print-directory -j8

.PHONY: all clean SignedDistance.so


all: clean
	$(make) SDFlib.a CXXFLAGS="$(CXXFLAGS) -fPIC"
	$(make) SDFcad

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o Common.o shader.o SignedDistanceHelper.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@


SDFlib.a: SDFlib.o MarchingCubes.o Common.o
	ar rvs $@ $^

SignedDistance.so: SDFlib.a
	$(CXX) -shared -fPIC $(CXXFLAGS) -Wl,--whole-archive $^ -Wl,--no-whole-archive $(SDF_FILE) -o $@


clean :
	rm -f SDFcad *.o *.a *.so

