using namespace std;
#include "rectangle.hpp"


Rectangle::Rectangle(const Point& position, float w, float h, const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor)
         : position(position), w(w), h(h), frameColor(frameColor), fillColor(fillColor) {}

Rectangle::~Rectangle() = default;


// ###################  Methods  ###################

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

void Rectangle::draw() const {
  auto [up_left, down_right] = diag_coor();
  al_draw_filled_rectangle(up_left.x, up_left.y, down_right.x, down_right.y, fillColor);
  al_draw_rectangle(up_left.x, up_left.y, down_right.x, down_right.y, frameColor, 1);
}
