#include "vecteur.hpp"


Vecteur::Vecteur(const Point& position, int w, int h)
       : w(w), h(h), position(position),
         top_vec(), bottom_vec(), left_vec(), right_vec(), edges(),
         top(), bottom(), left(), right(), lines(),
         intersectionPoint({0, 0}),
         last_deplacement(calculateLineEquation({0, 0}, {0, 0})) {
  updateEdges();
  updateLines();
  edges = {top_vec, bottom_vec, left_vec, right_vec};
  lines = {top, bottom, left, right};
}

void Vecteur::updateEdges() {
  float half_w = static_cast<float>(w) / 2.0f;
  float half_h = static_cast<float>(h) / 2.0f;
  Point top_left = {position.x - half_w, position.y - half_h};
  Point top_right = {position.x + half_w, position.y - half_h};
  Point bottom_left = {position.x - half_w, position.y + half_h};
  Point bottom_right = {position.x + half_w, position.y + half_h};
  top_vec = make_pair(top_left, top_right);
  bottom_vec = make_pair(bottom_left, bottom_right);
  left_vec = make_pair(top_left, bottom_left);
  right_vec = make_pair(top_right, bottom_right);
}

void Vecteur::updateLines() {
  top = calculateLineEquation(top_vec.first, top_vec.second);
  bottom = calculateLineEquation(bottom_vec.first, bottom_vec.second);
  left = calculateLineEquation(left_vec.first, left_vec.second);
  right = calculateLineEquation(right_vec.first, right_vec.second);
}

[[gnu::pure]] bool Vecteur::sameEquationLine(const Line& line_1, const Line& line_2) const {
  constexpr float eps = numeric_limits<float>::epsilon();
  return (fabs(line_1.m - line_2.m) <= eps && fabs(line_1.b - line_2.b) <= eps);
}

[[gnu::pure]] bool Vecteur::isOnSegment(const Point& p, const Point& q, const Point& inter) const {
  return (inter.x >= min(p.x, q.x) && inter.x <= max(p.x, q.x) &&
          inter.y >= min(p.y, q.y) && inter.y <= max(p.y, q.y));
}

[[gnu::pure]] Line Vecteur::calculateLineEquation(const Point& p, const Point& q) const {
  float slope, y_intercept;
  constexpr float eps = numeric_limits<float>::epsilon();
  if (fabs(p.x - q.x) <= eps) {
    slope = INFINITY;
    y_intercept = p.x;
  } else {
    slope = (q.y - p.y) / (q.x - p.x);
    y_intercept = p.y - slope * p.x;
  }
  return Line(slope, y_intercept);
}

[[gnu::pure]] float Vecteur::distance(const Point& p, const Point& q) const {
  return hypot(q.x - p.x, q.y - p.y);
}

[[gnu::pure]] Point Vecteur::minimalDistance(const vector<Point>& points, const Point& reference) const {
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

bool Vecteur::_intersection(const Line& deplacement, const Line& edge, const pair<Point, Point>& deplacement_vec, const pair<Point, Point>& edge_vec) {
  constexpr float eps = numeric_limits<float>::epsilon();
  if (fabs(deplacement.m - edge.m) <= eps) { return false; }

  float inter_x, inter_y;
  if (isinf(deplacement.m)) {
    inter_x = deplacement.b;
    inter_y = edge.m * inter_x + edge.b;
  } else if (isinf(edge.m)) {
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

int Vecteur::intersection(const pair<Point, Point>& deplacement_vec) {
  // Point deplacement_vec_ordered = trierPoints(deplacement_vec);
  Line deplacement = calculateLineEquation(deplacement_vec.first, deplacement_vec.second);
  last_deplacement = deplacement;
  vector<Point> intersections = {};
  map<Point, int> pointValues;
  // Tester chaque bord du rectangle
  for (int i = 0; i < 4; ++i) {
    if (_intersection(deplacement, lines.at(static_cast<size_t>(i)), deplacement_vec, edges.at(static_cast<size_t>(i)))) {
      intersections.push_back(intersectionPoint);
      pointValues[intersectionPoint] = (i < 2) ? 0 : 1;
    }
  }
  if (intersections.size()) { return pointValues[minimalDistance(intersections, deplacement_vec.first)]; } 
  else { return -1;}
}
