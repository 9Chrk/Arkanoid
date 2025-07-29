#pragma once
#include "model/utils.hpp"


struct Point {
  float x, y;
  Point(float x=0, float y=0) : x(x), y(y) {}

  bool operator==(const Point &other) const {
    constexpr float eps = std::numeric_limits<float>::epsilon();
    return fabs(x - other.x) <= eps && fabs(y - other.y) <= eps;
  }
  bool operator!=(const Point &other) const {
    return !(*this == other);
  }
  // for the object “map” to be valid
  bool operator<(const Point &other) const {
    constexpr float eps = std::numeric_limits<float>::epsilon();
    if (fabs(x - other.x) > eps) return x < other.x;
    return y < other.y;
  }
};

class Rectangle {
 protected:
  Point position;
  float w, h;

 public:
  Rectangle();
  Rectangle(const Point& position, float w, float h);

  // Methods
  bool contains(const Point& p)       const;
  std::pair<Point, Point> diag_coor() const;

  // Getters
  Point getPosition() const;
  float getWidth()    const;
  float getHeight()   const;
};
