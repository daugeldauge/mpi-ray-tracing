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
      if (arg.compare("-w") == 0) {
        width = std::stoi(argv[++i]);
      } else if (arg.compare("-h") == 0) {
        height = std::stoi(argv[++i]);
      } else if (arg.compare("-f") == 0) {
        path = argv[++i];
      }
    }
    std::cout << width << " " << height << std::endl;

    Camera camera = {
      {-70.f, 15.f, 0.f},  /* position */
      
      {1.f, 0.f, 0.f},      /* forward */
      {0.f, 0.f, -1.f},     /* right */
      {0.f, -1.f, 0.f},     /* up */
      
      {1.04720f, 0.817275f} /* view angle */
    };

    Tracer tracer = {camera, width, height, 20};
    tracer.renderImage(path);
  
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl; 
  }
  
  MPI::Finalize();
  return 0;
}