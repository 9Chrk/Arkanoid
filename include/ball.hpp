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

  void draw();
  void move(Point spaceship, int spaceship_height);
  void move(Point spaceship, float w, float h);
  Point getPosition();
  Point getDirection();
  bool inMouvement();
  bool isFalling();
  void setMouvement(bool cas);
  void setFalling(bool cas);
  void setDirection(Point newDirection);
  float getRayon();
  void checkCollisions(Point spaceship, float w, float h);
  void checkFall();
  void reset();
};

#endif
