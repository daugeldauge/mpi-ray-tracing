#ifndef TRACER_H
#define TRACER_H

#include <string>

#include "scene.h"

class Tracer
{
  Scene scene;
  int width, height;

public:
  Tracer(const Scene &scene, int width, int height)
    : scene(scene), width(width), height(height) {}

  void renderImage();
  void saveImage(std::string path);
};

#endif