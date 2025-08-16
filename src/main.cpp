#include <exception>
#include <iostream>
#include "engine/Engine.hpp"


int main() {
  try {
    Engine{}.run();
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
