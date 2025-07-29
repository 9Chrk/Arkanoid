#pragma once
#include "model/utils.hpp"
#include "Rectangle.hpp"


class Vector {
 private:

  struct Line {
    float m, b;
    Line(float slope=0, float y_intercept=0) : m(slope), b(y_intercept) {}
  };

  float w, h;
  Point position;

  std::pair<Point, Point> top_vec, bottom_vec, left_vec, right_vec;
  std::vector<std::pair<Point, Point>> edges;

  Line top, bottom, left, right;
  std::vector<Line> lines;
  
  Point intersectionPoint;

  // Update methods
  void updateEdges();
  void updateLines();

  // Internal geometric tools
  Line calculateLineEquation(const Point& p, const Point& q) const;
  bool sameEquationLine(const Line& line_1, const Line& line_2) const;

  // Intersection detection
  bool _intersection(const Line& deplacement, const Line& edge, const std::pair<Point, Point>& deplacement_vec, const std::pair<Point, Point>& edge_vec);
  bool isOnSegment(const Point& p, const Point& q, const Point& inter) const;
  Point minimalDistance(const std::vector<Point>& points, const Point& reference) const;

 public:
  Vector(const Point& position, float w, float h);

  // Static utility functions
  static float distance(const Point& p, const Point& q);
  static Point normalize(const Point& p);

  // Intersection detection
  std::pair<Point, int> intersection(const std::pair<Point, Point>& deplacement_vec);
};
