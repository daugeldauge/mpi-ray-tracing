#ifndef TRACER_H
#define TRACER_H

#include <string>

#include "scene.h"
#include "image.h"

struct Ray
{
  glm::vec3 start;
  glm::vec3 direction;
};

struct Camera
{
  glm::vec3 position;
  
  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;

  glm::vec2 viewAngle;
};

class Tracer
{
  Camera camera;
  Scene scene;
  Image image;
public:
  Tracer(Camera camera, int width, int height)
    : camera(camera), scene(), image(width, height) {}

  void renderImage(const std::string &path);

};

#endif