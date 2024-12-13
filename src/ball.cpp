#include "ball.hpp"

Ball::Ball(Point position, int rayon, int vitesse, ALLEGRO_COLOR color)
    : position(position), rayon(rayon), vitesse(vitesse), color(color), inMouvement(false), isFalling(false), d({0, -1}) {}

void Ball::draw() {
  al_draw_circle(position.x, position.y, rayon, color, rayon * 2);
}

void Ball::move(Point spaceship) {
  position.x = spaceship.x;
  position.y = spaceship.y - rayon * 3.5;
}

void Ball::move(Point spaceship, float w, float h) {
  checkCollisions(spaceship, w, h);
  checkFall();
}

Point Ball::getPosition() { return position; }
Point Ball::getDirection() { return d; }
void Ball::setDirection(Point newDirection) { d = newDirection; }
int Ball::getRayon() { return rayon; }

void Ball::checkCollisions(Point spaceship, float w, float h) {
  Point newPos = {position.x + d.x * vitesse, position.y + d.y * vitesse};
  
  if (newPos.x - rayon <= 0 || newPos.x + rayon >= windowWidth) { d.x *= -1; }
  if (newPos.y - rayon <= 0) { d.y *= -1; }
  
  if (newPos.y + rayon >= spaceship.y - h / 2 &&
      newPos.y + rayon <= spaceship.y + h / 2 + vitesse &&
      newPos.x >= spaceship.x - w / 2 &&
      newPos.x <= spaceship.x + w / 2) {
  
    float x_rel = (newPos.x - spaceship.x) / (w / 2);
    x_rel = clamp(x_rel, -1.0f, 1.0f);
  
    float alpha = 30 + 120 * (1 - x_rel);
    alpha = clamp(alpha, 30.0f, 150.0f);
    float theta = alpha * M_PI / 180.0f;
  
    d.x = cos(theta);
    d.y = -sin(theta);
  }
  position = newPos;
}

void Ball::checkFall() {
  isFalling = (position.y - rayon > windowHeight) ? true : false;
}

void Ball::reset() {
  d = {0, -1};
  inMouvement = false;
}
