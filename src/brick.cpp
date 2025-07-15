#include "brick.hpp"


Brick::Brick(const Point& position, float w, float h, const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor, int score)
     : Rectangle(position, w, h, frameColor, fillColor),
       score(score), destroyed(false), secondLife(score == 200),
       bonus(BonusType::NONE), vec(position, static_cast<int>(w), static_cast<int>(h)) {}

// Methods
void Brick::destroy() { destroyed = true; }


// ###################  Getters  ###################

[[gnu::pure]] bool Brick::getSecondLife() const { return secondLife; }
[[gnu::pure]] int Brick::getScore()       const { return score;      }
[[gnu::pure]] bool Brick::isDestroyed()   const { return destroyed;  }
[[gnu::pure]] Point Brick::getPosition()  const { return position;   }
[[gnu::pure]] float Brick::getWidth()     const { return w;          }
[[gnu::pure]] float Brick::getHeight()    const { return h;          }
[[gnu::pure]] BonusType Brick::getBonus() const { return bonus;      }


// ###################  Setters  ###################

void Brick::setSecondLife(bool cas)                   { secondLife = cas;   }
void Brick::setFrameColor(const ALLEGRO_COLOR& color) { frameColor = color; }
void Brick::setBonus(BonusType bonus)                 { bonus = bonus;      }
