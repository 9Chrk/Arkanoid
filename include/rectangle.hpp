#pragma once
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "common.hpp"
using namespace std;

struct Point {
  float x = 0, y = 0;
  Point() = default;
  Point(float x, float y) : x(x), y(y) {}

  bool operator<(const Point &other) const {   // pour que la def "map" soit valide
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
  Rectangle(Point position, float w, float h,
            ALLEGRO_COLOR frameColor = BLACK,
            ALLEGRO_COLOR fillColor = WHITE);
            
  void draw();
  bool contains(Point p);
  pair<Point, Point> diag_coor();
};

#endif
