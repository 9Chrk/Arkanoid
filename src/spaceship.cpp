#include "spaceship.hpp"

Spaceship::Spaceship(Point position, float w, float h, int vitesse, int health, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor)
    : Rectangle(position, w, h, frameColor, fillColor), health(health), reset_pos(position), vitesse(vitesse) {}

void Spaceship::move(int direction) {
    Point newPos = position;
    newPos.x += (direction == 0 ? -vitesse : vitesse);
    position.x = clamp(newPos.x, w / 2.0f, windowWidth - w / 2.0f);
}

void Spaceship::move(Point mousePosition) {
    Point newPos = position;
    newPos.x = mousePosition.x;
    if (validPosition(newPos) && mousePosition.y >= windowHeight * 2 / 3) {
        position = newPos;
    }
}

bool Spaceship::validPosition(Point newPos) {
    return (newPos.x - w / 2 >= 0) && (newPos.x + w / 2 <= windowWidth);
}

Point Spaceship::getPosition() { return position; }
float Spaceship::getWidth() { return w; }
float Spaceship::getHeight() { return h; }
int Spaceship::getHealth() { return health; }
void Spaceship::damage() { health--; }
bool Spaceship::isDeath() { return health <= 0; }
void Spaceship::reset() { position = reset_pos; }

