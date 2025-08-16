using namespace std;
#include "model/Spaceship.hpp"


Spaceship::Spaceship()
    : Rectangle({0.0f, 0.0f}, 0, 0), health(0), speed(0), laserActive(false) {}

Spaceship::Spaceship(const Point& position, float w, float h, float speed, int health)
    : Rectangle(position, w, h), health(health), speed(speed), laserActive(false) {}

         
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

void Spaceship::setWidth(float newWidth) {
  w = newWidth;
  position.x = clamp(position.x, w/2, GAME_WIDTH - w/2);
}

void Spaceship::addLife() { health++; }

void Spaceship::setLaser(bool enabled) { laserActive = enabled; }

[[gnu::pure]] bool Spaceship::validPosition(const Point& newPos) const {
  return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= GAME_WIDTH); // check horizontal bounds
}

void Spaceship::damage() { health--; } // lose one life

// Getters

[[gnu::pure]] int  Spaceship::getHealth() const { return health;      }
[[gnu::pure]] bool Spaceship::isDeath()   const { return health <= 0; }
[[gnu::pure]] bool  Spaceship::hasLaser() const { return laserActive; }
[[gnu::pure]] float Spaceship::getSpeed() const { return speed; }
