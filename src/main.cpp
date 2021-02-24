#include <iostream>
#include <cstdlib>

#include "config.h"
#include "tui/main.h"

int main(int argc, char* argv[]) {
  std::cout << "CVD - CoolVideoDownloader v" << VERSION_MAJOR
    << "." << VERSION_MINOR << std::endl;

  if(std::getenv("DISPLAY")) {
    std::cout << "Warning: GUI is under development." << std::endl;
  }

  tui_start();

  return 0;
}
