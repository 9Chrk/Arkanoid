#include "brick.hpp"

Brick::Brick(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor, int score)
     : Rectangle(position, w, h, frameColor, fillColor), score(score), destroyed(false), vec(position, w, h) {}

int Brick::getScore() { return score; }

int Brick::destroy() {
  destroyed = true;
  return score;
}

[[gnu::pure]]
bool Brick::isDestroyed() {
  return destroyed;
}
