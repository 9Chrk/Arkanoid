#include "vecteur.hpp"


Vecteur::Vecteur(const Point& position, int w, int h)
        : position(position), w(w), h(h), intersectionPoint({0, 0}) {
  last_deplacement = calculateLineEquation({0, 0}, {0, 0});
  updateEdges();
  updateLines();
  edges = {top_vec, bottom_vec, left_vec, right_vec};
  lines = {top, bottom, left, right};
}

void Vecteur::updateEdges() {
  Point top_left = {position.x - w / 2.0f, position.y - h / 2.0f};
  Point top_right = {position.x + w / 2.0f, position.y - h / 2.0f};
  Point bottom_left = {position.x - w / 2.0f, position.y + h / 2.0f};
  Point bottom_right = {position.x + w / 2.0f, position.y + h / 2.0f};
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
  return (line_1.m == line_2.m && line_1.b == line_2.b);
}

[[gnu::pure]] bool Vecteur::isOnSegment(const Point& p, const Point& q, const Point& inter) const {
  return (inter.x >= min(p.x, q.x) && inter.x <= max(p.x, q.x) &&
          inter.y >= min(p.y, q.y) && inter.y <= max(p.y, q.y));
}

[[gnu::pure]] Line Vecteur::calculateLineEquation(const Point& p, const Point& q) const {
  float slope, y_intercept;
  if (p.x == q.x) {
    slope = INFINITY;
    y_intercept = p.x;
  } else {
    slope = (q.y - p.y) / (q.x - p.x);
    y_intercept = p.y - slope * p.x;
  }
  return Line(slope, y_intercept);
}

[[gnu::pure]] float Vecteur::distance(const Point& p, const Point& q) const {
  return sqrt(pow(q.x - p.x, 2) + pow(q.y - p.y, 2));
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
  if (deplacement.m == edge.m) { return false; }

  float inter_x, inter_y;
  if (deplacement.m == INFINITY) {
    inter_x = deplacement.b;
    inter_y = edge.m * inter_x + edge.b;
  } else if (edge.m == INFINITY) {
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
    if (_intersection(deplacement, lines.at(i), deplacement_vec, edges.at(i))) {
      intersections.push_back(intersectionPoint);
      pointValues[intersectionPoint] = (i < 2) ? 0 : 1;
    }
  }
  if (intersections.size()) { return pointValues[minimalDistance(intersections, deplacement_vec.first)]; }
  else { return -1;}
}

// ---- additional geometry helpers ----

float dot(Vec2 a, Vec2 b) {
  return a.x * b.x + a.y * b.y;
}

float length2(Vec2 v) {
  return dot(v, v);
}

Vec2 normalize(Vec2 v) {
  float len = std::sqrt(length2(v));
  if (len > 0.f) { v.x /= len; v.y /= len; }
  return v;
}

Vec2 reflect(Vec2 v, Vec2 n) {
  float d = dot(v, n) * 2.f;
  return {v.x - d * n.x, v.y - d * n.y};
}

Vec2 clamp(Vec2 p, Vec2 min, Vec2 max) {
  return {std::clamp(p.x, min.x, max.x), std::clamp(p.y, min.y, max.y)};
}

bool sweepCircleAABB(const Vec2& c0, const Vec2& vel,
                     float radius,
                     const Rect& box,
                     float tMax,
                     float& outTOI, Vec2& outN)
{
  Rect exp = box;
  exp.inflate(radius);

  Vec2 invDir = {1.0f / vel.x, 1.0f / vel.y};
  float t1 = (exp.x - c0.x) * invDir.x;
  float t2 = (exp.x + exp.w - c0.x) * invDir.x;
  float t3 = (exp.y - c0.y) * invDir.y;
  float t4 = (exp.y + exp.h - c0.y) * invDir.y;

  float tNear = std::max(std::min(t1, t2), std::min(t3, t4));
  float tFar  = std::min(std::max(t1, t2), std::max(t3, t4));

  if (tNear > tFar || tFar < 0.f || tNear > tMax) return false;

  outTOI = std::max(tNear, 0.f);
  if      (tNear == t1) outN = {-1.f, 0.f};
  else if (tNear == t2) outN = { 1.f, 0.f};
  else if (tNear == t3) outN = { 0.f,-1.f};
  else                  outN = { 0.f, 1.f};
  return true;
}
