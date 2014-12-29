CXX = mpicxx
CXXFLAGS = -g -std=c++11
LDFLAGS =

HFILES = tracer.h scene.h
CPPFILES = main.cpp tracer.cpp scene.cpp
OBJECTS = $(CPPFILES:.cpp=.o)

TARGET = raytracer

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

include deps.make
deps.make: $(CPPFILES) $(HFILES)
	gcc -MM $(CPPFILES) > deps.make

clean:
	rm -f $(TARGET) deps.make *.o