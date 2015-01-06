#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "tiny_obj_loader.h"

#include "image.h"

struct Triangle
{
  glm::vec3 v0;
  glm::vec3 v1;
  glm::vec3 v2;
  Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2)
    : v0(v0), v1(v1), v2(v2) {}
  Triangle() {}
};

struct Shape : public tinyobj::shape_t
{
  int nVertices() const { return mesh.positions.size() / 3; }
  int nTriangles() const { return mesh.indices.size() / 3; }
  
  glm::vec3 vertex(int i) const
  {
    return glm::make_vec3(mesh.positions.data() + 3 * i);
  };
  
  Triangle triangle(int i) const {
    return Triangle(
      vertex(mesh.indices[3 * i + 0]),
      vertex(mesh.indices[3 * i + 1]),
      vertex(mesh.indices[3 * i + 2])
    );
  }

  Shape &operator=(const tinyobj::shape_t &tinyShape)
  {
    name = tinyShape.name;
    mesh = tinyShape.mesh;
    material = tinyShape.material;
    return *this;
  }
};

struct Scene
{
  std::vector<Shape> shapes;
  Scene(const std::string &path);
};

#endif
