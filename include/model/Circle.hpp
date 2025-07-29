#pragma once
#include "model/utils.hpp"
#include "Rectangle.hpp"


class Circle {
 protected:
  Point position;
  float radius;

 public:
  Circle(const Point& position, float radius);

  // Getters
  Point getPosition() const;
  float getRadius() const;

  // Setters
  void setPosition( const Point& newPosition);
};
