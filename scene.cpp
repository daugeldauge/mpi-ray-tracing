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
  std::copy(tinyShapes.begin(), tinyShapes.end(), shapes.begin());

}