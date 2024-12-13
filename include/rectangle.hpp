#pragma once
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "common.hpp"
using namespace std;

struct Point {
  float x = 0, y = 0;
};

class Rectangle {
 protected:
  Point position;
  float w, h;
  ALLEGRO_COLOR frameColor, fillColor;
  pair<Point, Point> diag_coor();

 public:
  Rectangle(Point position, float w, float h,
            ALLEGRO_COLOR frameColor = BLACK,
            ALLEGRO_COLOR fillColor = WHITE);
  void draw();
  bool contains(Point p);
};

#endif
