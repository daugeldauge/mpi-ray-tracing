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
  data = std::unique_ptr<glm::vec3[]>(new glm::vec3[width * height]);
  
  each([&](glm::vec3 &pixel, int i, int j) {
    pixel.r = image[4 * width * i + 4 * j + 0] / 255.0f;
    pixel.g = image[4 * width * i + 4 * j + 1] / 255.0f;
    pixel.b = image[4 * width * i + 4 * j + 2] / 255.0f;
  });
}

void
Image::generateSample()
{
  each([this](glm::vec3 &pixel, int i, int) {
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

  each([&](const glm::vec3 &pixel, int i, int j) {
    image[4 * width * i + 4 * j + 0] = glm::clamp(pixel.r, 0.f, 1.f) * 255.f; 
    image[4 * width * i + 4 * j + 1] = glm::clamp(pixel.g, 0.f, 1.f) * 255.f; 
    image[4 * width * i + 4 * j + 2] = glm::clamp(pixel.b, 0.f, 1.f) * 255.f;
    image[4 * width * i + 4 * j + 3] = 255;
  });

  unsigned error = lodepng::encode(path, image, width, height);
  if (error) {
    throw std::runtime_error(std::string("lodepng encoding error: ") + lodepng_error_text(error));
  }
}