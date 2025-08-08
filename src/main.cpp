using namespace std;
#include "Engine.hpp"


int main() {
  try {
    Engine{}.run();
    return 0;
  } 
  catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }
}
