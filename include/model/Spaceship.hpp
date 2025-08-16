#pragma once
#include "core/utils.hpp"


class Spaceship : public Rectangle {
 private:
  int health;
  float speed;
  bool laserActive;

 public:
  Spaceship();
  Spaceship(const Point& position, float w, float h, float speed, int health);

  // Methods
  void move(Direction dir);
  void move(const Point& mousePosition);

  void setWidth(float newWidth);
  void addLife();
  void setLaser(bool enabled);
  
  bool validPosition(const Point& position) const;
  void damage();

  // Getters
  int getHealth() const;
  bool isDeath()  const;
  bool hasLaser() const;
  float getSpeed() const;
};
