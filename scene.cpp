#include <iostream>

#include "l3ds.h"

#include "scene.h"

void
Scene::load(const std::string &path)
{
  L3DS l3ds(path.c_str());
  
  meshes.resize(l3ds.GetMeshCount());
  
  for (size_t i = 0; i < meshes.size(); ++i) {
    
    auto mesh = l3ds.GetMesh(i);
    meshes[i].resize(mesh.GetTriangleCount());
    
    for (size_t j = 0; j < meshes[i].size(); ++j) {
      
      auto toGlm = [](auto vec) {
        return glm::vec3(vec.x, vec.y, vec.z); 
      };

      auto triangle = mesh.GetTriangle2(j);
      auto a = toGlm(triangle.vertices[0]);
      auto b = toGlm(triangle.vertices[1]);
      auto c = toGlm(triangle.vertices[2]);

      meshes[i][j] = std::make_tuple(a, b, c);
    }
  }
}