CXX = mpicxx
CXXFLAGS = -g -std=c++11 -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS =

HFILES = tracer.h scene.h image.h lodepng.h
CPPFILES = main.cpp tracer.cpp scene.cpp image.cpp lodepng.cpp
OBJECTS = $(CPPFILES:.cpp=.o)

TARGET = raytracer

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

include deps.make
deps.make: $(CPPFILES) $(HFILES)
	$(CXX) -MM $(CXXFLAGS) $(CPPFILES) > deps.make

clean:
	rm -f $(TARGET) deps.make *.o