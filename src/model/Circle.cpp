#include "Circle.hpp"


Circle::Circle(const Point& position, float radius) : position(position), radius(radius) {}

// Getters

[[gnu::pure]] Point Circle::getPosition() const { return position; }
[[gnu::pure]] float Circle::getRadius()   const { return radius;   }

// Setters

void Circle::setPosition(const Point& newPosition) { position = newPosition; }
