#pragma once
#ifndef BONUS_HPP
#define BONUS_HPP

#include "common.hpp"
#include "rectangle.hpp"

enum class BonusType {
  NONE,
  LASER,
  EXPAND,
  CATCH,
  SLOW_DOWN,
  SPLIT,
  EXTRA_LIFE,
};


// ################### Bonus class ###################

class Bonus : public Rectangle {
 private:
 BonusType type;
 bool active;
 float fallSpeed;

 public:
  Bonus(const Point& position, float w, float h,
        const ALLEGRO_COLOR& frameColor,
        const ALLEGRO_COLOR& fillColor,
        BonusType type, float fallSpeed);

  // Methods
  void update();
  void draw() const override;
  
  // Getters
  bool isActive()      const;
  BonusType getType()  const;
  float getFallSpeed() const;

  // Setters
  void setActive(bool active);
  void setFallSpeed(float speed);
};

#endif
