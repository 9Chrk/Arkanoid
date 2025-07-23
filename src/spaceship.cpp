using namespace std;
#include "spaceship.hpp"
 

Spaceship::Spaceship() : Rectangle({0.0f, 0.0f}, 0, 0, BLACK, BLACK), health(0), vitesse(0) {}

Spaceship::Spaceship(const Point& position, float w, float h, int vitesse, int health,
                     const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor)
         : Rectangle(position, w, h, frameColor, fillColor), health(health), vitesse(vitesse) {}

         
// ###################  Methods  ###################

void Spaceship::move(int direction) {
  Point newPos = position;
  newPos.x += (direction == 0 ? -static_cast<float>(vitesse) : static_cast<float>(vitesse));
  position.x = clamp(newPos.x, w/2.0f, windowWidth - w/2.0f);
}

void Spaceship::move(const Point& mousePosition) {
  Point newPos = position;
  newPos.x = mousePosition.x;
  if (validPosition(newPos) && mousePosition.y >= windowHeight * playableArea) { position = newPos; }
}

[[gnu::pure]] bool Spaceship::validPosition(const Point& newPos) const {
  return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= windowWidth);
}

void Spaceship::damage() { health--; }


// ###################  Getters  ###################

[[gnu::pure]] Point Spaceship::getPosition() const { return position;    }
[[gnu::pure]] float Spaceship::getWidth()    const { return w;           }
[[gnu::pure]] float Spaceship::getHeight()   const { return h;           }
[[gnu::pure]] int Spaceship::getHealth()     const { return health;      }
[[gnu::pure]] bool Spaceship::isDeath()      const { return health <= 0; }
