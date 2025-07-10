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

  void move(int direction);
  void move(const Point& mousePosition);
  [[nodiscard]] bool validPosition(const Point& position) const;
  [[nodiscard]] Point getPosition() const;
  [[nodiscard]] float getWidth() const;
  [[nodiscard]] float getHeight() const;
  [[nodiscard]] int getHealth() const;
  void damage();
  [[nodiscard]] bool isDeath() const;
};

#endif
