#pragma once
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "common.hpp"
using namespace std;


struct Point {
  float x = 0, y = 0;
  Point() = default;
  Point(float x, float y) : x(x), y(y) {}

  // pour que la def "map" soit valide
  bool operator<(const Point &other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }
  
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point &other) const {
    return !(*this == other);
  }
};

class Rectangle {
 protected:
  Point position;
  float w, h;
  ALLEGRO_COLOR frameColor, fillColor;

 public:
  Rectangle(const Point& position, float w, float h,
            const ALLEGRO_COLOR& frameColor = BLACK,
            const ALLEGRO_COLOR& fillColor = WHITE);
            
  void draw() const;
  bool contains(const Point& p) const;
  pair<Point, Point> diag_coor() const;
};

#endif
