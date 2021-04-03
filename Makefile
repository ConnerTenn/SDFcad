CC = gcc
CXX = g++


CFLAGS = -Wall -O3
CXXFLAGS = -Wall -O3
LDFLAGS = -lm
LDFLAGS += $(shell pkg-config --static --libs glfw3)
LDFLAGS += $(shell pkg-config --static --libs glew)

TARGET = SDFcad
cpp_files = SDFcad.cpp shader.cpp
objects = $(cpp_files:.cpp=.o)

make = $(MAKE) --no-print-directory -j8

all: clean
	$(make) RayMarch.fragshader
	$(make) $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(TARGET): $(objects) 
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

RayMarch.fragshader:
	cpp -P RayMarch.frag -o $@

.PHONY : clean
clean :
	rm -f $(TARGET) RayMarch.fragshader $(objects)

