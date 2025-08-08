#pragma once
#include "core/utils.hpp"
#include "core/Circle.hpp"


class Ball : public Circle {
 private:
  Point direction;
  float speed;
  bool moving, falling;

 public:
  Ball();
  Ball(float radius, float speed);

  // Movement
  void move(const Point& spaceship, float h);
  void move(const Point& spaceship, float w, float h);
  
  // Collision and state management
  void checkCollisions(const Point& spaceship, float w, float h);
  void checkFall();
  void reset();
  
  // Getters
  Point getDirection() const;
  float getSpeed()     const;
  
  bool inMouvement()   const;
  bool isFalling()     const;
  
  // Setters
  void setMouvement(bool cas);
  void setFalling(  bool cas);
  void setDirection(const Point& newDirection);
};
