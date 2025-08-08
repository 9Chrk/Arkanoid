#pragma once
#include "core/utils.hpp"


class Spaceship : public Rectangle {
 private:
  int health, vitesse;

 public:
  Spaceship();
  Spaceship(const Point& position, float w, float h, int vitesse, int health);

  // Methods
  void move(int direction);
  void move(const Point& mousePosition);
  
  bool validPosition(const Point& position) const;
  void damage();

  // Getters
  int getHealth()     const;
  bool isDeath()      const;
};
