#include <iostream>

#include "mpi.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "tracer.h"

using namespace glm;

inline Ray
Tracer::createRay(int i, int j) const
{
  float x = float(j) / image.width  * 2.f - 1.f; 
  float y = float(i) / image.height * 2.f - 1.f; 

  vec3 direction = normalize(camera.forward + rightHalfPlane * x + upHalfPlane * y);

  return {camera.position, direction};
}

inline float
Ray::isIntersect(const Triangle &triangle) const
{
  /* http://ray-tracing.ru/articles213.html */
  
  vec3 v0, v1, v2;

  std::tie(v0, v1, v2) = triangle;

  vec3 D = direction;
  vec3 O = start;

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

inline std::tuple<glm::vec3, bool>
Ray::refract(glm::vec3 normal, float index) const
{
  float eta = 1.0f / index;
  float cosTheta = -dot(normal, direction);
  if (cosTheta < 0) {
    cosTheta *= -1.0f;
    normal *= -1.0f;
    eta = 1.0f / eta;
  }
  float k = 1.0f - eta * eta * (1.0f - cosTheta * cosTheta);
  
  vec3 refraction = direction;
  if (k >= 0.0f) {
    refraction = normalize(eta * direction + (eta * cosTheta - sqrt(k)) * normal);
  }
  return std::make_tuple(refraction, (k > 0.0f));
}

inline float
fresnelReflectance(float ci, float n)
{
  if (ci < 0) {
    n = 1.0f / n;
  }
  float ci2 = ci * ci;
  float si2 = 1.0f - ci2;
  float si4 = si2 * si2;
  float a = ci2 + n * n - 1.0f;
  float sqa = 2 * sqrtf(a) * ci;
  float b = ci2 + a;
  float c = ci2 * a + si4;
  float d = sqa * si2;
  float r = (b - sqa) / (b + sqa) * (1.0f + (c - d) / (c + d)) * 0.5f;
  // if (r > 1.0f || r < 0.0f ) {
  //   throw std::runtime_error("Invalid fresnel reflectance coefficient: " + std::to_string(r));
  // }
  return r;
}

inline vec3 pow(float base, const vec3 &exp)
{
  return vec3(
    pow(base, exp.x),
    pow(base, exp.y),
    pow(base, exp.z)
  );
}

inline vec3 mult(const vec3 &a, const vec3 &b)
{
  return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline glm::vec3
Tracer::handleIntersection(const Ray &ray, const tinyobj::material_t &material, const Triangle &triangle)
{
  vec3 pixel;
  vec3 v0, v1, v2;
  std::tie(v0, v1, v2) = triangle;

  vec3 normal = normalize(cross(v1 - v0, v2 - v0));

  vec3 v = v0 - ray.start;
  float d = dot(normal, v);
  float e = dot(normal, ray.direction);

  vec3 point = ray.start + ray.direction * d / e;
  
  { /* Phong lightning model */
    vec3 s = normalize(camera.position - point);
  
    auto ka = make_vec3(material.ambient);
    auto kd = make_vec3(material.diffuse);
    auto ks = make_vec3(material.specular);
    auto ke = make_vec3(material.emission);
    
    float diffuseIntensity = clamp(dot(s, normal), 0.0f, 1.0f);
    vec3 lightReflection = normalize(reflect(s, normal));
    vec3 specularIntensity = pow(clamp(dot(lightReflection, ray.direction), 0.0f, 1.0f), ke);

    pixel = ka + kd * diffuseIntensity + mult(ks, specularIntensity); 
  }
  
  vec3 refracted(1.f, 1.f, 1.f);
  if (material.ior > 1.f) {
    Ray refraction, reflection = {point, reflect(ray.direction, normal)}; 
    bool isRefraction;
    std::tie(refraction.direction, isRefraction) = ray.refract(normal, material.ior);

    if (isRefraction) {
      refraction.start = point;
      float r = fresnelReflectance(-e, material.ior);
      if (r >= 1.f){
        refracted = traceRay(refraction);
      } else {
        refracted = (1 - r) * traceRay(refraction) + r * traceRay(reflection);
      }
    } else {
      refracted = traceRay(reflection);
    }
  }
  pixel = mult(pixel, refracted);


  return clamp(pixel, 0.0f, 1.0f);
}

// glm::vec3
// Tracer::getTextureColor(const Ray &ray)
// {
//   const float radius = 100.0f;
//   float b = dot(ray.start, ray.direction);
//   float c = dot(ray.start, ray.start) - radius * radius;
//   float d = b * b - c; 

//   if (d < 0) {
//     throw std::runtime_error("Your sphere was fucked up!");
//   }
//   float sqrtd = sqrtf(d);

//   float t1 = -b + sqrtd;
//   float t2 = -b - sqrtd;

//   float minT = std::min(t1, t2);
//   float maxT = std::max(t1, t2);

//   float t = (minT >= 0)? minT : maxT;

//   vec3 point = ray.start + t * ray.direction;

//   float theta = acos(point.y / l2Norm(point)); 
//   float phi = atan(point.z, point.x);

//   int i = (theta * scene.texture.height / pi<float>());
//   int j = ((phi + pi<float>()) * scene.texture.height * one_over_pi<float>() / 2);

//   return scene.texture(i, j);
// }

glm::vec3
Tracer::traceRay(const Ray &ray)
{
  if (++depth > maxDepth) {
    return {1.f, 0.f, 0.f};
  }
  
  float minDistance = std::numeric_limits<float>::max();
  
  bool isIntersection = false;
  Triangle triangle;
  tinyobj::material_t material;

  for (auto shape : scene.shapes) {
    for (int i = 0; i < shape.nTriangles(); ++i) {
      float distance = ray.isIntersect(shape.triangle(i));
      
      if (distance > 0.f && distance < minDistance) {
        isIntersection = true;
        minDistance = distance;
        triangle = shape.triangle(i);
        material = shape.material;
      }
    }
  }

  if (isIntersection) {
    return handleIntersection(ray, material, triangle);
  } else {
    return {0.f, 0.f, 0.f};
  }

}

void
Tracer::renderImage(const std::string &path)
{ 

  rightHalfPlane = camera.right * glm::tan(camera.viewAngle.x / 2.f);
     upHalfPlane = camera.up    * glm::tan(camera.viewAngle.y / 2.f);

  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  int blockSize = image.width * image.height / size;

  for (int blockIndex = 0; blockIndex < blockSize; ++blockIndex) {
    int globalIndex = rank * blockSize + blockIndex;
    int i = globalIndex / image.width;
    int j = globalIndex % image.width;
    Ray ray = createRay(i, j);
    depth = 0;
    image(i, j) = traceRay(ray);
  }   

  image.gather();
  image.save(path);
}