#include "ball.hpp"


Ball::Ball() : rayon(0), vitesse(0), color(BLACK), position({0, 0}), d({0, -1}), mouvement(false), falling(false) {}

Ball::Ball(float rayon, float vitesse, ALLEGRO_COLOR color)
    : rayon(rayon), vitesse(vitesse), color(color), position({0, 0}), d({0, -1}), mouvement(false), falling(false) {}

// Drawing and movement

void Ball::draw() const {
  al_draw_circle(position.x, position.y, rayon, color, rayon * 2);
}

void Ball::move(const Point& spaceship, int spaceship_height) {
  position.x = spaceship.x;
  position.y = spaceship.y - spaceship_height - rayon;
}


// Getters
[[gnu::pure]] Point Ball::getPosition()  const { return position; }
[[gnu::pure]] Point Ball::getDirection() const { return d; }
[[gnu::pure]] float Ball::getRayon()     const { return rayon; }
[[gnu::pure]] float Ball::getVitesse()   const { return vitesse; }
[[gnu::pure]] bool  Ball::inMouvement()  const { return mouvement; }
[[gnu::pure]] bool  Ball::isFalling()    const { return falling; }

// Setters
void Ball::setDirection(const Point& newDirection) { d = newDirection; }
void Ball::setMouvement(bool cas) { mouvement = cas; }
void Ball::setFalling(bool cas) { falling = cas; }
void Ball::setPosition(const Point& newPos) { position = newPos; }

// Collision and state management

  
void Ball::checkFall() {
  falling = (position.y - rayon > windowHeight) ? true : false;
}
  
void Ball::reset() {
  d = {0, -1};
  mouvement = false;
}
  