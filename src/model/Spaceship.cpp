#include "Spaceship.hpp"


Spaceship::Spaceship() : Rectangle({0.0f, 0.0f}, 0, 0), health(0), vitesse(0) {}

Spaceship::Spaceship(const Point& position, float w, float h, int vitesse, int health)
         : Rectangle(position, w, h), health(health), vitesse(vitesse) {}

         
// Methods

void Spaceship::move(int direction) {
  Point newPos = position;
  newPos.x += (direction == 0 ? -static_cast<float>(vitesse) : static_cast<float>(vitesse));
  position.x = std::clamp(newPos.x, w/2, GameWidth - w/2);
}

void Spaceship::move(const Point& mousePosition) {
  Point newPos = position;
  newPos.x = mousePosition.x;
  if (validPosition(newPos) && mousePosition.y >= GameHeight * PlayableRatio) { position = newPos; }
}

[[gnu::pure]] bool Spaceship::validPosition(const Point& newPos) const {
  return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= GameWidth);
}

void Spaceship::damage() { health--; }

// Getters

[[gnu::pure]] int Spaceship::getHealth() const { return health;      }
[[gnu::pure]] bool Spaceship::isDeath()  const { return health <= 0; }
