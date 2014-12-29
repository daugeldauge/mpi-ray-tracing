#include <string>
#include <iostream>

#include "mpi.h"

#include "tracer.h"



int 
main(int argc, char *argv[])
{ 
  MPI::Init(argc, argv);

  try {
    int width = 640, height = 480;
    std::string path = "result.png";
    
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i]; 
      if (arg.compare("-w")) {
        width = std::stoi(argv[++i]);
      } else if (arg.compare("-h")) {
        height = std::stoi(argv[++i]);
      } else if (arg.compare("-f")) {
        path = argv[++i];
      }
    }
    Camera camera = {
      {-10.f, 0.f, 3.f}, //position
      {  1.f, 0.f, 0.f}, //forward
      {  0.f, 1.f, 0.f}, //right
      {  0.f, 0.f, 1.f}, //up
      {1.04720f, 0.817275f} //view angle
    };

    Tracer tracer = {camera, width, height};
    tracer.renderImage(path);
  
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl; 
  }
  
  MPI::Finalize();
  return 0;
}