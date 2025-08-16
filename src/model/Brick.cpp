
#include "model/Brick.hpp"


Brick::Brick(const Point& position, float w, float h, int score, BonusType bonus)
     : Rectangle(position, w, h), score(score), destroyed(false), secondLife(score == SECOND_LIFE),
       bonus(bonus), vec(position, w, h) {}

// Methods

void Brick::destroy() { destroyed = true; } // mark brick as destroyed

// Getters

[[gnu::pure]] bool Brick::getSecondLife() const { return secondLife; }
[[gnu::pure]] int Brick::getScore()       const { return score;      }
[[gnu::pure]] bool Brick::isDestroyed()   const { return destroyed;  }
[[gnu::pure]] BonusType Brick::getBonus() const { return bonus;      }
[[gnu::pure]] Vector Brick::getVector()   const { return vec;        }

// Setters

void Brick::setSecondLife(bool cas)       { secondLife = cas;  }
void Brick::setBonus(BonusType bonusType) { bonus = bonusType; }
