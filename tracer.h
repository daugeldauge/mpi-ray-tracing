#ifndef TRACER_H
#define TRACER_H

#include <string>

#include "scene.h"
#include "image.h"

class Tracer
{
  Scene scene;
  Image image;
public:
  Tracer(int width, int height)
    : scene(), image(width, height) {}

  void renderImage(const std::string &path);
};

#endif