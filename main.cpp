#include <string>

#include "mpi.h"

#include "tracer.h"



int 
main(int argc, char *argv[])
{
  MPI::Init(argc, argv);

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

  Tracer tracer = {width, height};
  tracer.renderImage(path);

  MPI::Finalize();
  return 0;
}