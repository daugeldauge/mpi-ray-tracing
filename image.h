#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "glm/glm.hpp"

class Image
{
public:
  int width, height;
  std::unique_ptr<glm::vec3[]> data;
  
  Image(int width, int height)
    : width(width), height(height), data(new glm::vec3[width * height]) {}
  
  glm::vec3 &operator()(int i, int j) { return data[width * i + j]; }
  const glm::vec3 &operator()(int i, int j) const { return data[width * i + j]; }
  void save(const std::string &path) const;
  void generateSample();
  
  template <typename Function> void
  each(Function function)
  {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        function((*this)(i, j), i, j);
      }
    }
  }
  
  template <typename Function> void
  each(Function function) const
  {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        function((*this)(i, j), i, j);
      }
    }
  }

};

#endif