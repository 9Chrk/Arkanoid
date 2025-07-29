#include "Vector.hpp"


Vector::Vector(const Point& position, float w, float h)
       : w(w), h(h), position(position),
         top_vec(), bottom_vec(), left_vec(), right_vec(), edges(),
         top(), bottom(), left(), right(), lines(),
         intersectionPoint({0, 0}) {
  updateEdges();
  updateLines();
  edges = {top_vec, bottom_vec, left_vec, right_vec};
  lines = {top, bottom, left, right};
}

// Update methods

void Vector::updateEdges() {
  Point top_left     = {position.x - w/2, position.y - h/2};
  Point top_right    = {position.x + w/2, position.y - h/2};
  Point bottom_left  = {position.x - w/2, position.y + h/2};
  Point bottom_right = {position.x + w/2, position.y + h/2};

  top_vec    = std::make_pair(top_left, top_right);
  bottom_vec = std::make_pair(bottom_left, bottom_right);
  left_vec   = std::make_pair(top_left, bottom_left);
  right_vec  = std::make_pair(top_right, bottom_right);
}

void Vector::updateLines() {
  top    = calculateLineEquation(top_vec.first, top_vec.second);
  bottom = calculateLineEquation(bottom_vec.first, bottom_vec.second);
  left   = calculateLineEquation(left_vec.first, left_vec.second);
  right  = calculateLineEquation(right_vec.first, right_vec.second);
}

// Internal geometric tools

[[gnu::pure]] Vector::Line Vector::calculateLineEquation(const Point& p, const Point& q) const {
  float slope, y_intercept;
  constexpr float eps = std::numeric_limits<float>::epsilon();
  if (std::fabs(p.x - q.x) <= eps) {
    slope = INFINITY;
    y_intercept = p.x;
  } else {
    slope = (q.y - p.y) / (q.x - p.x);
    y_intercept = p.y - slope * p.x;
  }
  return Line(slope, y_intercept);
}

[[gnu::pure]] bool Vector::sameEquationLine(const Line& line_1, const Line& line_2) const {
  constexpr float eps = std::numeric_limits<float>::epsilon();
  return (std::fabs(line_1.m - line_2.m) <= eps && std::fabs(line_1.b - line_2.b) <= eps);
}

// Intersection detection

std::pair<Point, int> Vector::intersection(const std::pair<Point, Point>& deplacement_vec) {
  Line deplacement = calculateLineEquation(deplacement_vec.first, deplacement_vec.second);
  std::vector<Point> intersections = {};
  std::map<Point, int> pointValues;
  
  // Test each edge of the rectangle
  for (int i = 0; i < 4; ++i) {
    if (_intersection(deplacement, lines.at(static_cast<size_t>(i)), deplacement_vec, edges.at(static_cast<size_t>(i)))) {
      int side = (i < 2) ? 0 : 1;
      auto it = pointValues.find(intersectionPoint);
      if (it == pointValues.end()) {
        intersections.push_back(intersectionPoint);
        pointValues[intersectionPoint] = side;
      } else if (it->second != side) {
        it->second = 2;
      }
    }
  }
  if (intersections.empty()) return {{0,0}, -1};

  Point closestIntersection = minimalDistance(intersections, deplacement_vec.first);
  if (intersections.size()) return {closestIntersection, pointValues[closestIntersection]};
  else return {{0, 0}, -1};
}

[[gnu::pure]] bool Vector::isOnSegment(const Point& p, const Point& q, const Point& inter) const {
  return (inter.x >= std::min(p.x, q.x) && inter.x <= std::max(p.x, q.x) &&
          inter.y >= std::min(p.y, q.y) && inter.y <= std::max(p.y, q.y));
}

[[gnu::pure]] Point Vector::minimalDistance(const std::vector<Point>& points, const Point& reference) const {
  if (points.size() == 1) {
    return points.at(0);
  }
  Point min_point = points.at(0);
  float min_distance = distance(reference, points.at(0));
  
  for (Point point : points) {
    float current_distance = distance(reference, point);
    if (current_distance < min_distance) {
      min_distance = current_distance;
      min_point = point;
    }
  }
  return min_point;
}

//  Helper methods

bool Vector::_intersection(const Line& deplacement, const Line& edge, const std::pair<Point, Point>& deplacement_vec, const std::pair<Point, Point>& edge_vec) {
  constexpr float eps = std::numeric_limits<float>::epsilon();
  if (std::fabs(deplacement.m - edge.m) <= eps) { return false; }

  float inter_x, inter_y;
  if (std::isinf(deplacement.m)) {
    inter_x = deplacement.b;
    inter_y = edge.m * inter_x + edge.b;
  } else if (std::isinf(edge.m)) {
    inter_x = edge.b;
    inter_y = deplacement.m * inter_x + deplacement.b;
  } else {
    inter_x = (edge.b - deplacement.b) / (deplacement.m - edge.m);
    inter_y = deplacement.m * inter_x + deplacement.b;
  }
  intersectionPoint = Point(inter_x, inter_y);

  return isOnSegment(deplacement_vec.first, deplacement_vec.second, intersectionPoint) &&
         isOnSegment(edge_vec.first, edge_vec.second, intersectionPoint);
}

//  Static utility functions

[[gnu::pure]] float Vector::distance(const Point& p, const Point& q) {
  return std::hypot(q.x - p.x, q.y - p.y);
}

Point Vector::normalize(const Point& p){
  constexpr float eps = std::numeric_limits<float>::epsilon();
  float norm = std::sqrt(p.x * p.x + p.y * p.y);
  if (norm < eps) return {0,0};
  return {p.x / norm, p.y / norm};
}
