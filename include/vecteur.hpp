#pragma once
#ifndef VECTEUR_HPP
#define VECTEUR_HPP

#include "common.hpp"
#include "rectangle.hpp"


struct Line {
  float m = 0, b = 0;
  Line() = default;
  Line(float slope, float y_intercept) : m(slope), b(y_intercept) {}
};

class Vecteur {
 private:
  int w, h;
  Point position;

  pair<Point, Point> top_vec, bottom_vec, left_vec, right_vec;
  vector<pair<Point, Point>> edges;

  Line top, bottom, left, right;
  vector<Line> lines;

  Point intersectionPoint;
  Line last_deplacement;

  void updateEdges();
  void updateLines();

  float distance(const Point& p, const Point& q) const;
  Point minimalDistance(const vector<Point>& points, const Point& reference) const;

  Line calculateLineEquation(const Point& p, const Point& q) const;
  bool sameEquationLine(const Line& line_1, const Line& line_2) const;
  bool _intersection(const Line& deplacement, const Line& edge, const pair<Point, Point>& deplacement_vec, const pair<Point, Point>& edge_vec);
  bool isOnSegment(const Point& p, const Point& q, const Point& inter) const;

 public:
  Vecteur(const Point& position, int w, int h);

  int intersection(const pair<Point, Point>& deplacement_vec);
};

#endif
