CXX = mpicxx
CXXFLAGS = -g -O3 -Werror
LDFLAGS =

HFILES = tracer.h scene.h image.h lodepng.h tiny_obj_loader.h
CPPFILES = main.cpp tracer.cpp scene.cpp image.cpp lodepng.cpp tiny_obj_loader.cpp
OBJECTS = $(CPPFILES:.cpp=.o)

TARGET = raytracer

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

include deps.make
deps.make: $(CPPFILES) $(HFILES)
	$(CXX) -MM $(CXXFLAGS) $(CPPFILES) > deps.make

clean:
	rm -f $(TARGET) deps.make *.o result.png