CXX = mpicxx
CXXFLAGS = -g -std=c++11
LDFLAGS =

HFILES = tracer.h scene.h image.h
CPPFILES = main.cpp tracer.cpp scene.cpp image.cpp
OBJECTS = $(CPPFILES:.cpp=.o)

INCLUDES = -I./libs/
CXXFLAGS += $(INCLUDES)

TARGET = raytracer

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

include deps.make
deps.make: $(CPPFILES) $(HFILES)
	$(CXX) -MM $(CXXFLAGS) $(CPPFILES) > deps.make

clean:
	rm -f $(TARGET) deps.make *.o