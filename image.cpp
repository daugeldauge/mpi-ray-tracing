#include <vector>
#include <stdexcept>

#include "lodepng.h"

#include "image.h"

Image::Image(const std::string &path)
{
  std::vector<unsigned char> image;

  unsigned w, h;
  unsigned error = lodepng::decode(image, w, h, path);
  if (error) {
    throw std::runtime_error(std::string("lodepng decoding error: ") + lodepng_error_text(error));
  }
  width = w;
  height = h;
  data = new glm::vec3[width * height];
  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      glm::vec3 &pixel = (*this)(i, j);
      pixel.r = image[4 * width * i + 4 * j + 0] / 255.0f;
      pixel.g = image[4 * width * i + 4 * j + 1] / 255.0f;
      pixel.b = image[4 * width * i + 4 * j + 2] / 255.0f;
    }
  }
}

void
Image::generateSample()
{
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      glm::vec3 &pixel = (*this)(i, j);
      if (i < height / 3) {
        pixel = glm::vec3(1.f, 0.f, 0.f);
      } else if (i < 2 * height / 3) {
        pixel = glm::vec3(0.f, 1.f, 0.f);
      } else {
        pixel = glm::vec3(0.f, 0.f, 1.f);
      }
    }
  }
}

void
Image::save(const std::string &path) const
{
  if (MPI::COMM_WORLD.Get_rank() != 0) {
    return;
  }
  std::vector<unsigned char> image(width * height * 4);

  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      const glm::vec3 &pixel = (*this)(i, j);
      image[4 * width * i + 4 * j + 0] = static_cast<unsigned char>(glm::clamp(pixel.r, 0.f, 1.f) * 255.f);
      image[4 * width * i + 4 * j + 1] = static_cast<unsigned char>(glm::clamp(pixel.g, 0.f, 1.f) * 255.f);
      image[4 * width * i + 4 * j + 2] = static_cast<unsigned char>(glm::clamp(pixel.b, 0.f, 1.f) * 255.f);
      image[4 * width * i + 4 * j + 3] = static_cast<unsigned char>(255.f);
    }
  }

  unsigned error = lodepng::encode(path, image, width, height);
  if (error) {
    throw std::runtime_error(std::string("lodepng encoding error: ") + lodepng_error_text(error));
  }
}

void
Image::gather()
{
  glm::vec3 *newData = 0;
  if (MPI::COMM_WORLD.Get_rank() == 0) { 
    newData = new glm::vec3[width * height];
  }
  size_t size = blockSize * sizeof(glm::vec3);
  MPI::COMM_WORLD.Gather(data, size, MPI::BYTE, newData, size, MPI::BYTE, 0);
  if (data) {
    delete[] data;
  }
  data = newData;
}
