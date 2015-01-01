#ifndef TRACER_H
#define TRACER_H

#include <string>

#include "scene.h"
#include "image.h"

struct Ray
{
  glm::vec3 start;
  glm::vec3 direction;

  float isIntersect(const Triangle &triangle) const;
  std::tuple<glm::vec3, bool> refract(glm::vec3 normal, float index) const;
};

struct Camera
{
  glm::vec3 position;
  
  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;

  glm::vec2 viewAngle; // in radians
  
  Camera(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec2 &viewAngle)
    : position(position), forward(glm::normalize(forward)), viewAngle(viewAngle)
  {
    right = glm::cross(forward, glm::vec3(0.f, -1.f, 0.f));
    up = glm::cross(right, forward);
  }
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

  glm::vec3 handleIntersection(const Ray &ray, const tinyobj::material_t &material, const Triangle &triangle);
  glm::vec3 getTextureColor(const Ray &ray);


public:
  Tracer(const std::string &path, Camera camera, int width, int height, int maxDepth)
    : camera(camera), scene(path), 
      image(width, height), maxDepth(maxDepth) {}

  void renderImage(const std::string &path);
};

#endif