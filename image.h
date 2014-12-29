#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "glm/glm.hpp"

class Image
{
  int width, height;
public:
  std::unique_ptr<glm::vec3[]> data;
  
  Image(int width, int height)
    : width(width), height(height), data(new glm::vec3[width * height]) {}
  
  glm::vec3 &operator()(int i, int j) { return data[width * i + j]; }
  const glm::vec3 &operator()(int i, int j) const { return data[width * i + j]; }
  void save(const std::string &path) const;
  template <typename Function> void each(Function function);
  template <typename Function> void each(Function function) const;
  void generateSample();
};

#endif