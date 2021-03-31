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

all: clean $(TARGET)

$(TARGET): $(objects) 
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean :
	-rm $(TARGET) $(objects)

