using namespace std;
#include "Spaceship.hpp"


Spaceship::Spaceship() : Rectangle({0.0f, 0.0f}, 0, 0), health(0), speed(0) {}

Spaceship::Spaceship(const Point& position, float w, float h, float speed, int health)
         : Rectangle(position, w, h), health(health), speed(speed) {}

         
// Methods

void Spaceship::move(Direction dir) {
  Point newPos = position;
  newPos.x += (dir == Direction::Left ? -speed : speed);
  position.x = clamp(newPos.x, w/2, GAME_WIDTH - w/2); // keep inside screen
}

void Spaceship::move(const Point& mousePosition) {
  Point newPos = position;
  newPos.x = mousePosition.x;
  if (validPosition(newPos) && mousePosition.y >= GAME_HEIGHT * PLAYABLE_RATIO) { position = newPos; } // follow mouse in lower area
}

[[gnu::pure]] bool Spaceship::validPosition(const Point& newPos) const {
  return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= GAME_WIDTH); // check horizontal bounds
}

void Spaceship::damage() { health--; } // lose one life

// Getters

[[gnu::pure]] int  Spaceship::getHealth() const { return health;      }
[[gnu::pure]] bool Spaceship::isDeath()   const { return health <= 0; }
