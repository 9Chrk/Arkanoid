#pragma once
#ifndef BALL_HPP
#define BALL_HPP

#include "common.hpp"
#include "spaceship.hpp"

class Ball {
 private:
   float rayon, vitesse;
   ALLEGRO_COLOR color;
   Point position, d;
   bool mouvement, falling;

 public:
  Ball();
  Ball(float rayon, float vitesse, ALLEGRO_COLOR color);

  void draw() const;
  void move(const Point& spaceship, int spaceship_height);
  void move(const Point& spaceship, float w, float h);
  [[nodiscard]] Point getPosition() const;
  [[nodiscard]] Point getDirection() const;
  [[nodiscard]] bool inMouvement() const;
  [[nodiscard]] bool isFalling() const;
  void setMouvement(bool cas);
  void setFalling(bool cas);
  void setDirection(const Point& newDirection);
  [[nodiscard]] float getRayon() const;
  [[nodiscard]] float getVitesse() const;
  void checkCollisions(const Point& spaceship, float w, float h);
  void checkFall();
  void reset();
};

#endif
