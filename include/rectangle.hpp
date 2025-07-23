#pragma once
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "common.hpp"

struct Point {
  float x = 0, y = 0;
  Point() = default;
  Point(float x, float y) : x(x), y(y) {}

  bool operator==(const Point &other) const {
    constexpr float eps = std::numeric_limits<float>::epsilon();
    return fabs(x - other.x) <= eps && fabs(y - other.y) <= eps;
  }

  bool operator!=(const Point &other) const {
    return !(*this == other);
  }

  // for the def “map” to be valid
  bool operator<(const Point &other) const {
    constexpr float eps = std::numeric_limits<float>::epsilon();
    if (fabs(x - other.x) > eps) return x < other.x;
    return y < other.y;
  }
};


// ################### Rectangle class ###################

class Rectangle {
 protected:
  Point position;
  float w, h;
  ALLEGRO_COLOR frameColor, fillColor;

 public:
  Rectangle(const Point& position, float w, float h,
            const ALLEGRO_COLOR& frameColor = BLACK,
            const ALLEGRO_COLOR& fillColor = WHITE);
  
  Rectangle(Rectangle&&)                 = default;
  Rectangle(const Rectangle&)            = default;
  Rectangle& operator=(Rectangle&&)      = default;
  Rectangle& operator=(const Rectangle&) = default;

  virtual ~Rectangle();

  virtual void draw()                 const;
  bool contains(const Point& p)       const;
  std::pair<Point, Point> diag_coor() const;
};

#endif
