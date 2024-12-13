#include "brick.hpp"

Brick::Brick(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor, int scores)
     : Rectangle(position, w, h, frameColor, fillColor), scores(scores), destroyed(false), vec(position, w, h) {}

int Brick::destroy() {
  destroyed = true;
  return scores;
}

bool Brick::isDestroyed() {
  return destroyed;
}
