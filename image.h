#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <memory>

#include "mpi.h"
#include "glm/glm.hpp"

class Image
{
public:
  int offset;
  int blockSize;
  int width, height;
  glm::vec3 *data;
  
  Image(int width, int height): width(width), height(height)
  {
    blockSize = width * height / MPI::COMM_WORLD.Get_size();
    offset = blockSize * MPI::COMM_WORLD.Get_rank();
    data = new glm::vec3[blockSize];
  }

  Image(const std::string &path);
  
  ~Image()
  {
    if (data) {
      delete[] data;
    }
  }

  glm::vec3 &operator()(int i, int j) { return data[width * i + j - offset]; }
  const glm::vec3 &operator()(int i, int j) const { return data[width * i + j - offset]; }
  void gather();
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
