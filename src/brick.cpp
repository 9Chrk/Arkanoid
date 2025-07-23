using namespace std;
#include "brick.hpp"


Brick::Brick(const Point& position, float w, float h, const ALLEGRO_COLOR& frameColor, const ALLEGRO_COLOR& fillColor, int score, BonusType bonus)
     : Rectangle(position, w, h, frameColor, fillColor),
       score(score), destroyed(false), secondLife(score == 200),
       bonus(bonus), vec(position, static_cast<int>(w), static_cast<int>(h)) {}

// Methods
void Brick::destroy() { destroyed = true; }

void Brick::draw() const {
  Rectangle::draw();
  if (bonus != BonusType::NONE) {
    string txt = getBonusAbbreviation(bonus);
    float text_width  = static_cast<float>(al_get_text_width(font_bonus, txt.c_str()));
    float text_height = static_cast<float>(al_get_font_line_height(font_bonus));

    float draw_x = position.x - text_width/2.0f;
    float draw_y = position.y - text_height/2.0f;

    al_draw_text(font_bonus, BLACK, draw_x, draw_y, 0, txt.c_str());
  }
}


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
