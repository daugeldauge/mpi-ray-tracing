#include <vector>
#include <stdexcept>

#include "lodepng.h"

#include "image.h"

template <typename Function> void
Image::each(Function function) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      function((*this)(i, j), i, j);
    }
  }
}

void
Image::generateSample()
{
  each([this](glm::vec3 &pixel, int i, int j){
    if (i < height / 3) {
      pixel = glm::vec3(1.f, 0.f, 0.f);
    } else if (i < 2 * height / 3) {
      pixel = glm::vec3(0.f, 1.f, 0.f);
    } else {
      pixel = glm::vec3(0.f, 0.f, 1.f);
    }
  });
}

void
Image::save(const std::string &path) const
{
  std::vector<unsigned char> image(width * height * 4);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      auto pixel = (*this)(i, j);
      image[4 * width * i + 4 * j + 0] = glm::clamp(pixel.r, 0.f, 1.f) * 255.f; 
      image[4 * width * i + 4 * j + 1] = glm::clamp(pixel.g, 0.f, 1.f) * 255.f; 
      image[4 * width * i + 4 * j + 2] = glm::clamp(pixel.b, 0.f, 1.f) * 255.f;
      image[4 * width * i + 4 * j + 3] = 255;
    }
  }

  unsigned error = lodepng::encode(path, image, width, height);
  if (error) {
    throw std::runtime_error(std::string("lodepng encoding error: ") + lodepng_error_text(error));
  }
}