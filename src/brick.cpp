#include "brick.hpp"

Brick::Brick(const Point& position, float w, float h, const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor, int score)
     : Rectangle(position, w, h, frameColor, fillColor), score(score), destroyed(false), vec(position, w, h)
     { secondLife = (score == 200) ? true : false; }

void Brick::destroy() { destroyed = true; }

void Brick::setSecondLife(bool cas) { secondLife = cas; }

[[gnu::pure]] bool Brick::getSecondLife() const { return secondLife; }
[[gnu::pure]] int Brick::getScore() const { return score; }
[[gnu::pure]] bool Brick::isDestroyed() const { return destroyed; }
[[gnu::pure]] Point Brick::getPosition() const { return position; }
[[gnu::pure]] float Brick::getWidth() const { return w; }
[[gnu::pure]] float Brick::getHeight() const { return h; }
