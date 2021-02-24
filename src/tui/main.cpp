#include <iostream>
#include <string>

#include "../api/main.h"

void tui_start() {
  std::string release;

  while(release.empty()) {
    std::cout << "Введите релиз: ";
    std::getline(std::cin, release);
  }

  std::cout << "Введено: " << release << std::endl
    << "Символов: " << release.size() << std::endl;

  std::cout << find(release) << std::endl;
}
