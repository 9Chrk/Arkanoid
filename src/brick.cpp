#include "brick.hpp"

Brick::Brick(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor, int score)
     : Rectangle(position, w, h, frameColor, fillColor), score(score), destroyed(false), vec(position, w, h) 
     { secondLife = (score == 200) ? true : false; }

void Brick::destroy() { destroyed = true; }

void Brick::setSecondLife(bool cas) { secondLife = cas; }

[[gnu::pure]] bool Brick::getSecondLife() { return secondLife; }
[[gnu::pure]] int Brick::getScore() { return score; }
[[gnu::pure]] bool Brick::isDestroyed() { return destroyed; }
[[gnu::pure]] Point Brick::getPosition() { return position; }
[[gnu::pure]] float Brick::getWidth() { return w; }
[[gnu::pure]] float Brick::getHeight() { return h; }
