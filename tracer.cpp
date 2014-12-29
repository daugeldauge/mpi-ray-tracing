#include "tracer.h"

void
Tracer::renderImage(const std::string &path)
{
	image.generateSample();
	image.save(path);
}