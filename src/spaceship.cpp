#include "spaceship.hpp"
 
Spaceship::Spaceship() : Rectangle({0.0f, 0.0f}, 0, 0, BLACK, BLACK), health(0), vitesse(0) {}

Spaceship::Spaceship(Point position, float w, float h, int vitesse, int health, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor)
         : Rectangle(position, w, h, frameColor, fillColor), health(health), vitesse(vitesse) {}

void Spaceship::move(int direction) {
  Point newPos = position;
  newPos.x += (direction == 0 ? -static_cast<float>(vitesse) : static_cast<float>(vitesse));
  position.x = clamp(newPos.x, w / 2.0f, windowWidth - w / 2.0f);
}

void Spaceship::move(Point mousePosition) {
  Point newPos = position;
  newPos.x = mousePosition.x;
  if (validPosition(newPos) && mousePosition.y >= windowHeight * 1 / 3) {
    position = newPos;
  }
}

[[gnu::pure]]
bool Spaceship::validPosition(Point newPos) {
  return (newPos.x - w / 2 >= 0) && (newPos.x + w / 2 <= windowWidth);
}

[[gnu::pure]] Point Spaceship::getPosition() { return position; }
[[gnu::pure]] float Spaceship::getWidth() { return w; }
[[gnu::pure]] float Spaceship::getHeight() { return h; }
[[gnu::pure]] int Spaceship::getHealth() { return health; }
[[gnu::pure]] bool Spaceship::isDeath() { return health <= 0; }

void Spaceship::damage() { health--; }
