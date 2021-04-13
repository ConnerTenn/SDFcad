
CC = gcc
CXX = g++

CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS = -lm
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

make = $(MAKE) --no-print-directory -j8

all: clean
	$(make) SDFcad

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

SDFcad: SDFcad.o Common.o shader.o MarchingCubes.o SignedDistance.o
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.PHONY : clean
clean :
	rm -f SDFcad *.o

