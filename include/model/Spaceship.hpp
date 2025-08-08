#pragma once
#include "core/utils.hpp"


class Spaceship : public Rectangle {
 private:
  int health;
  float speed;

 public:
  Spaceship();
  Spaceship(const Point& position, float w, float h, float speed, int health);

  // Methods
  void move(Direction dir);
  void move(const Point& mousePosition);
  
  bool validPosition(const Point& position) const;
  void damage();

  // Getters
  int getHealth() const;
  bool isDeath()  const;
};
