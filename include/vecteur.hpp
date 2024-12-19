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

  bool hasIntersection;
  Point intersectionPoint;
  Line last_deplacement;

  void updateEdges();
  void updateLines();

  float distance(Point p, Point q);
  Point minimalDistance(vector<Point> points, Point reference);

  Line calculateLineEquation(Point p, Point q);
  bool sameEquationLine(Line line_1, Line line_2);
  bool _intersection(Line deplacement, Line edge, pair<Point, Point> deplacement_vec, pair<Point, Point> edge_vec);
  bool isOnSegment(Point p, Point q, Point inter);

 public:
  Vecteur(Point position, int w, int h);

  int intersection(pair<Point, Point> deplacement_vec);
};

#endif
