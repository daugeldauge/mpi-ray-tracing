#include <iostream>

#include "scene.h"

Scene::Scene(const std::string &path)
{
  std::vector<tinyobj::shape_t> tinyShapes;
  
  std::string err = tinyobj::LoadObj(tinyShapes, path.c_str());
  if (!err.empty()) {
    throw std::runtime_error("Tiny obj loader error:" + err);
  }

  shapes.resize(tinyShapes.size());
  std::copy(begin(tinyShapes), end(tinyShapes), begin(shapes));

  for (auto &shape : shapes) {
    for (int i = 0; i < shape.nTriangles(); ++i) {
      glm::vec3 v0, v1, v2;
      std::tie(v0, v1, v2) = shape.triangle(i);
      std::cout << v0.x << " " << v0.y << " " << v0.z << std::endl;
      std::cout << v1.x << " " << v1.y << " " << v1.z << std::endl;
      std::cout << v2.x << " " << v2.y << " " << v2.z << std::endl << std::endl;
    }
  }
}