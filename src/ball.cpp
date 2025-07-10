#include "ball.hpp"

Ball::Ball() : rayon(0), vitesse(0), color(BLACK), position({0, 0}), d({0, -1}), mouvement(false), falling(false) {}

Ball::Ball(float rayon, float vitesse, ALLEGRO_COLOR color)
    : rayon(rayon), vitesse(vitesse), color(color), position({0, 0}), d({0, -1}), mouvement(false), falling(false) {}

void Ball::draw() const {
  al_draw_circle(position.x, position.y, rayon, color, rayon * 2);
}

void Ball::move(const Point& spaceship, int spaceship_height) {
  position.x = spaceship.x;
  position.y = spaceship.y - spaceship_height - rayon;
}

void Ball::move(const Point& spaceship, float w, float h) {
  checkCollisions(spaceship, w, h);
  checkFall();
}

void Ball::checkCollisions(const Point& spaceship, float w, float h) {
  Point newPos = {position.x + d.x * vitesse, position.y + d.y * vitesse};
  
  if (newPos.x - rayon <= 0 || newPos.x + rayon >= windowWidth) { d.x *= -1; }
  if (newPos.y - rayon <= 0) { d.y *= -1; }
  
  if (newPos.y + rayon >= spaceship.y - h / 2 &&
      newPos.y + rayon <= spaceship.y - h / 2 + vitesse &&
      newPos.x >= spaceship.x - w / 2 - rayon &&
      newPos.x <= spaceship.x + w / 2 + rayon) {
  
    float x_rel = (newPos.x - spaceship.x) / (w / 2);
    x_rel = clamp(x_rel, -1.0f, 1.0f);
  
    float alpha = 30 + 120 * (1 - x_rel);
    alpha = clamp(alpha, 30.0f, 150.0f);
    float theta = alpha * static_cast<float>(M_PI) / 180.0f;
  
    d.x = cos(theta);
    d.y = -sin(theta);
  }
  newPos.x = clamp(newPos.x, 0.0f, static_cast<float>(windowWidth));
  newPos.y = clamp(newPos.y, 0.0f, static_cast<float>(windowHeight)*1.5f);
  position = newPos;
}

void Ball::checkFall() {
  falling = (position.y - rayon > windowHeight) ? true : false;
}

void Ball::reset() {
  d = {0, -1};
  mouvement = false;
}

[[gnu::pure]] Point Ball::getPosition() const { return position; }
[[gnu::pure]] Point Ball::getDirection() const { return d; }
[[gnu::pure]] float Ball::getRayon() const { return rayon; }
[[gnu::pure]] float Ball::getVitesse() const { return vitesse; }
[[gnu::pure]] bool Ball::inMouvement() const { return mouvement; }
[[gnu::pure]] bool Ball::isFalling() const { return falling; }

void Ball::setDirection(const Point& newDirection) { d = newDirection; }
void Ball::setMouvement(bool cas) { mouvement = cas; }
void Ball::setFalling(bool cas) { falling = cas; }