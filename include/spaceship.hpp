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
  Spaceship(const Point& position, float w, float h, int vitesse, int health,
            const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor);

  // Getters
  Point getPosition() const;
  float getWidth()    const;
  float getHeight()   const;
  int getHealth()     const;
  bool isDeath()      const;

  // Other methods
  void move(int direction);
  void move(const Point& mousePosition);
  bool validPosition(const Point& position) const;
  void damage();
};

#endif
