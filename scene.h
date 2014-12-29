#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <tuple>

#include "glm/glm.hpp"

typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> Triangle;
typedef std::vector<Triangle> Mesh;

struct Scene
{
  std::vector<Mesh> meshes;
  void load(const std::string &path);
};

#endif