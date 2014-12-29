#include "tracer.h"

void
Tracer::renderImage(const std::string &path)
{
	scene.load("resources/crystal.3ds");
	image.generateSample();
	image.save(path);
}