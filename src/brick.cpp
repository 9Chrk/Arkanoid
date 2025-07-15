#include "brick.hpp"


Brick::Brick(const Point& position, float w, float h, const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor, int score)
     : Rectangle(position, w, h, frameColor, fillColor),
       score(score), destroyed(false), secondLife(score == 200),
       vec(position, static_cast<int>(w), static_cast<int>(h)) {}

// Actions
void Brick::destroy() { destroyed = true; }

// Getters
[[gnu::pure]] bool Brick::getSecondLife() const { return secondLife; }
[[gnu::pure]] int Brick::getScore()       const { return score; }
[[gnu::pure]] bool Brick::isDestroyed()   const { return destroyed; }
[[gnu::pure]] Point Brick::getPosition()  const { return position; }
[[gnu::pure]] float Brick::getWidth()     const { return w; }
[[gnu::pure]] float Brick::getHeight()    const { return h; }

// Setters
void Brick::setSecondLife(bool cas) { secondLife = cas; }
void Brick::setFrameColor(const ALLEGRO_COLOR& color) { frameColor = color; };
