#include <string>
#include <iostream>

//#include "mpi.h"

#include "tracer.h"



int 
main(int argc, char *argv[])
{ 
  //MPI::Init(argc, argv);

  try {
    int width = 640, height = 480;
    std::string path = "result.png";
    std::string obj = "resources/cube.obj";
    
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i]; 
      if (arg.compare("-w") == 0) {
        width = std::stoi(argv[++i]);
      } else if (arg.compare("-h") == 0) {
        height = std::stoi(argv[++i]);
      } else if (arg.compare("-result") == 0) {
        path = argv[++i];
      } else if (arg.compare("-obj") == 0) {
        obj = argv[++i];
      }
    }
    std::cout << width << " " << height << std::endl;

    Camera camera(
      {-2.f, -2.f, -1.f},  /* position */
      
      {1.f, 1.f, 1.f},      /* forward */

      {1.04720f, 0.817275f} /* view angle */
    );

    Tracer tracer = {obj, camera, width, height, 40};
    tracer.renderImage(path);
  
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl; 
  }
  
  //MPI::Finalize();
  return 0;
}