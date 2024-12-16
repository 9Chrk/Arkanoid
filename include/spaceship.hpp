#pragma once
#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "common.hpp"
#include "rectangle.hpp"

class Spaceship : public Rectangle {
 private:
  int health, vitesse;

 public:
  Spaceship();
  Spaceship(Point position, float w, float h, int vitesse, int health, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor);

  void move(int direction);
  void move(Point mousePosition);
  bool validPosition(Point position);
  Point getPosition();
  float getWidth();
  float getHeight();
  int getHealth();
  void damage();
  bool isDeath();
};

#endif
