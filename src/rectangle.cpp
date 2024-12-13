#include "rectangle.hpp"

Rectangle::Rectangle(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor)
         : position(position), w(w), h(h), frameColor(frameColor), fillColor(fillColor) {}

pair<Point, Point> Rectangle::diag_coor() {
  Point up_left = {position.x - w / 2, position.y - h / 2};
  Point down_right = {position.x + w / 2, position.y + h / 2};
  return {up_left, down_right};
}

void Rectangle::draw() {
  auto [up_left, down_right] = diag_coor();
  al_draw_filled_rectangle(up_left.x, up_left.y, down_right.x, down_right.y, fillColor);
  al_draw_rectangle(up_left.x, up_left.y, down_right.x, down_right.y, frameColor, 1);
}

bool Rectangle::contains(Point p) {
  auto [up_left, down_right] = diag_coor();
  return (up_left.x <= p.x && p.x <= down_right.x &&
          up_left.y <= p.y && p.y <= down_right.y);
}
