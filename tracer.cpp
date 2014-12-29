#include <iostream>

#include "tracer.h"

inline Ray
Tracer::createRay(int i, int j) const
{
  float x = (j + 0.5f) / image.width  * 2.0f - 1.0f; 
  float y = (i + 0.5f) / image.height * 2.0f - 1.0f; 

  glm::vec3 direction = glm::normalize(camera.forward + rightHalfPlane * x + upHalfPlane * y);

  return {camera.position, direction};
}

inline float
Tracer::isIntersect(const Ray &ray, const Triangle &triangle)
{
  /* http://ray-tracing.ru/articles213.html */
  
  using namespace glm;

  vec3 v0 = std::get<0>(triangle);
  vec3 v1 = std::get<1>(triangle);
  vec3 v2 = std::get<2>(triangle);
  
  vec3 D = ray.direction;
  vec3 O = ray.start;

  vec3 E1 = v1 - v0;
  vec3 E2 = v2 - v0;

  vec3 P = cross(D, E2);
  float det = dot(E1, P);

  if (std::abs(det) < std::numeric_limits<float>::epsilon()) {
    return -1.f;
  }

  float invDet = 1.f / det;
  vec3 T = O - v0;
  float u = dot(T, P) * invDet;

  if (u < 0.0f || u > 1.f) {
    return -1.f;
  }
  
  vec3 Q = cross(T, E1);
  float v = dot(D, Q) * invDet;

  if (v < 0.f || u + v > 1.f) {
    return -1.f;
  }

  float t = dot(E2, Q) * invDet;

  if (t < std::numeric_limits<float>::epsilon()) {
    return -1.f;
  }

  vec3 n = normalize(cross(v1 - v0, v2 - v0));

  return dot(n, v0 - O) / dot(n, D);
}

inline glm::vec3
Tracer::handleIntersection(const Ray &, const Mesh &, const Triangle &)
{
  return {0.f, 0.f, 1.f};
}

glm::vec3
Tracer::traceRay(const Ray &ray)
{
  if (++depth > maxDepth) {
    return {1.f, 0.f, 0.f};
  }
  
  float minDistance = std::numeric_limits<float>::max();
  bool isIntersection = false;
  Mesh targetMesh;
  Triangle targetTriangle;

  for (auto mesh : scene.meshes) {
    for (auto triangle : mesh) {
      float distance = isIntersect(ray, triangle);
      if (distance > 0.f && distance < minDistance) {
        isIntersection = true;
        targetTriangle = triangle;
        targetMesh = mesh;
      }
    }
  }

  if (isIntersection) {
    return handleIntersection(ray, targetMesh, targetTriangle);
  } else {
    return {0.7f, 0.7f, 0.f};
  }

}

void
Tracer::renderImage(const std::string &path)
{
  scene.load("resources/crystal.3ds");
  
  rightHalfPlane = camera.right * glm::tan(camera.viewAngle.x / 2);
     upHalfPlane = camera.up    * glm::tan(camera.viewAngle.y / 2);

  image.each([&](auto &pixel, int i, int j) {
    if (j == 0) {
      std::cout << "line " << i << std::endl;
    }
    Ray ray = createRay(i, j);
    depth = 0;
    pixel = traceRay(ray);
  });   

  image.save(path);
}