#include "bonus.hpp"


Bonus::Bonus(const Point& position, float w, float h, 
             const ALLEGRO_COLOR& frameColor,
             const ALLEGRO_COLOR& fillColor, 
             BonusType type, float fallSpeed)
     : Rectangle(position, w, h, frameColor, fillColor), type(type), active(false), fallSpeed(fallSpeed) {}
 

 // ###################  Methods  ###################

 void Bonus::update() {
   if (active) { position.y += fallSpeed; }
 }
 
 void Bonus::draw() const {
   if (active) {
     float rayon = w/2.0f;
     Point left_arc  = {position.x - rayon, position.y};
     Point right_arc = {position.x + rayon, position.y};
 
     al_draw_filled_circle(left_arc.x, left_arc.y, rayon, frameColor);
     al_draw_filled_circle(right_arc.x, right_arc.y, rayon, frameColor);
     Rectangle::draw();
   }
 }


// ###################  Getters  ###################

[[gnu::pure]] BonusType Bonus::getType()      const { return type;      }
[[gnu::pure]] bool      Bonus::isActive()     const { return active;    }
[[gnu::pure]] float     Bonus::getFallSpeed() const { return fallSpeed; }


// ###################  Setters  ###################

void Bonus::setActive(bool isActive)  { active = isActive; }
void Bonus::setFallSpeed(float speed) { fallSpeed = speed; }
