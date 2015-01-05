#include <string>
#include <iostream>
#include <fstream>

#include "mpi.h"

#include "tracer.h"



int 
main(int argc, char *argv[])
{ 
  MPI::Init(argc, argv);

  double startTime = MPI::Wtime();

  try {
    int width = 640, height = 480;
    std::string path = "result.png";
    std::string obj;
    Camera camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.04720f, 0.817275f));

    std::ifstream config(argv[1]);


    while (!config.eof()) {
      std::string param;
      config >> param; 
      
      if (param.compare("-w") == 0) {
        config >> width;
      } else if (param.compare("-h") == 0) {
        config >> height;
      } else if (param.compare("-result") == 0) {
        config >> path;
      } else if (param.compare("-obj") == 0) {
        config >> obj;
      } else if (param.compare("-camera") == 0) {
        glm::vec3 position, forward;
        glm::vec2 viewAngle;
        config >> position.x >> position.y >> position.z;
        config >> forward.x >> forward.y >> forward.z;
        config >> viewAngle.x >> viewAngle.y;
        camera = Camera(position, forward, viewAngle);
      }
    }

    if (obj.empty()) {
      throw std::runtime_error("You must specify path to .obj file in config");
    }

    Tracer tracer(obj, camera, width, height, 40);
    tracer.renderImage(path);
  
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl; 
  }
  
  MPI::COMM_WORLD.Barrier();
  if (MPI::COMM_WORLD.Get_rank() == 0) {
    std::cout << "Execution time: " << MPI::Wtime() - startTime << std::endl;
  }
  MPI::Finalize();
  return 0;
}