using namespace std;
#include "Engine.hpp"

// Entry point of the program
int main(int /* argc */, char** /* argv */) {
  Engine engine;  // central game manager
  engine.run();   // start the main game loop

  return 0;
}
