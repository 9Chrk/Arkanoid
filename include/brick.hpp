#pragma once
#ifndef BRICK_HPP
#define BRICK_HPP

#include "common.hpp"
#include "rectangle.hpp"
#include "vecteur.hpp"

class Brick : public Rectangle {
 private:
  int score;
  bool destroyed, secondLife;

 public:
  Brick(Point position, float w, float h, 
        ALLEGRO_COLOR frameColor, 
        ALLEGRO_COLOR fillColor, 
        int score);

  Vecteur vec;
  void destroy();
  bool isDestroyed();
  bool getSecondLife();
  void setSecondLife(bool cas);
  int getScore();
  Point getPosition();
  float getWidth();
  float getHeight();
};

#endif
