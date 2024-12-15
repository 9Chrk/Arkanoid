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

 public:
  Ball();
  Ball(Point position, float rayon, float vitesse, ALLEGRO_COLOR color);

  bool inMouvement, isFalling;
  void draw();
  void move(Point spaceship);
  void move(Point spaceship, float w, float h);
  Point getPosition();
  Point getDirection();
  void setDirection(Point newDirection);
  float getRayon();
  void checkCollisions(Point spaceship, float w, float h);
  void checkFall();
  void reset();
};

#endif
