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

  glm::vec2 viewAngle; // in radians
};

class Tracer
{
  Camera camera;
  Scene scene;
  Image image;

  glm::vec3 rightHalfPlane;
  glm::vec3 upHalfPlane;

  int depth;
  int maxDepth;

  Ray createRay(int i, int j) const;
  glm::vec3 traceRay(const Ray &ray);

  float isIntersect(const Ray &ray, const Triangle &triangle);
  glm::vec3 handleIntersection(const Ray &ray, const Mesh &mesh, const Triangle &triangle);


public:
  Tracer(Camera camera, int width, int height, int maxDepth)
    : camera(camera), scene(), image(width, height), maxDepth(maxDepth) {}

  void renderImage(const std::string &path);
};

#endif