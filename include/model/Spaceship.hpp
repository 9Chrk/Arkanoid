#pragma once
#include "core/utils.hpp"


class Spaceship : public Rectangle {
 private:
  int health;
  float speed;
  bool laserActive;
  float originalWidth;
  float expandTimer;

 public:
  Spaceship();
  Spaceship(const Point& position, float w, float h, float speed, int health);

  // Methods
  void move(Direction dir);
  void move(const Point& mousePosition);

  void setWidth(float newWidth);
  void setLaser(bool enabled);
  void setExpandTimer(float time);
  
  bool validPosition(const Point& position) const;
  void damage();
  void addLife();
  void updateExpandTimer();

  // Getters
  int getHealth() const;
  bool isDeath()  const;
  bool hasLaser() const;
  float getSpeed() const;
  float getExpandTimer() const;
  float getoriginalWidth() const;
};
