using namespace std;
#include "Rectangle.hpp"


Rectangle::Rectangle() : position({0, 0}), w(0), h(0) {}

Rectangle::Rectangle(const Point& position, float w, float h)
         : position(position), w(w), h(h) {}

// Getters

[[gnu::pure]] Point Rectangle::getPosition() const { return position; }
[[gnu::pure]] float Rectangle::getWidth()    const { return w;        }
[[gnu::pure]] float Rectangle::getHeight()   const { return h;        }

// Methods

[[gnu::pure]] pair<Point, Point> Rectangle::diag_coor() const {
  Point up_left    = {position.x - w/2, position.y - h/2};
  Point down_right = {position.x + w/2, position.y + h/2};
  return {up_left, down_right};
}

[[gnu::pure]] bool Rectangle::contains(const Point& p) const {
  auto [up_left, down_right] = diag_coor();
  return (up_left.x <= p.x && p.x <= down_right.x &&
          up_left.y <= p.y && p.y <= down_right.y);
}
