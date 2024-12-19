#include "vecteur.hpp"

Vecteur::Vecteur(Point position, int w, int h)
        : position(position), w(w), h(h), hasIntersection(false), intersectionPoint({0, 0}) {
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

bool Vecteur::sameEquationLine(Line line_1, Line line_2) {
  return (line_1.m == line_2.m && line_1.b == line_2.b);
}

bool Vecteur::isOnSegment(Point p, Point q, Point inter) {
  return (inter.x >= min(p.x, q.x) && inter.x <= max(p.x, q.x) &&
          inter.y >= min(p.y, q.y) && inter.y <= max(p.y, q.y));
}

Line Vecteur::calculateLineEquation(Point p, Point q) {
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

float Vecteur::distance(Point p, Point q) {
  return sqrt(pow(q.x - p.x, 2) + pow(q.y - p.y, 2));
}

Point Vecteur::minimalDistance(vector<Point> points, Point reference) {
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


bool Vecteur::_intersection(Line deplacement, Line edge, pair<Point, Point> deplacement_vec, pair<Point, Point> edge_vec) {
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

int Vecteur::intersection(pair<Point, Point> deplacement_vec) {
  // Point deplacement_vec_ordered = trierPoints(deplacement_vec);
  Line deplacement = calculateLineEquation(deplacement_vec.first, deplacement_vec.second);
  
  if (!sameEquationLine(deplacement, last_deplacement)) {
    hasIntersection = false;
  }
  else if (sameEquationLine(deplacement, last_deplacement) && !hasIntersection) {
    return -1; 
  }
  last_deplacement = deplacement;
  vector<Point> intersections = {};
  map<Point, int> pointValues;
  // Tester chaque bord du rectangle
  for (int i = 0; i < 4; ++i) {
    if (_intersection(deplacement, lines.at(i), deplacement_vec, edges.at(i))) {
      hasIntersection = true;
      intersections.push_back(intersectionPoint);
      pointValues[intersectionPoint] = (i < 2) ? 0 : 1;
    }
  }
  return (hasIntersection) ? pointValues[minimalDistance(intersections, deplacement_vec.first)] : -1;
}