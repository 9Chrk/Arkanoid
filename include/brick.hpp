#pragma once
#ifndef BRICK_HPP
#define BRICK_HPP

#include "common.hpp"
#include "rectangle.hpp"
#include "vector.hpp"
#include "bonus_type.hpp"


class Brick : public Rectangle {
 private:
  int score;
  bool destroyed, secondLife;
  BonusType bonus;

 public:
  Brick(const Point& position, float w, float h,
        const ALLEGRO_COLOR& frameColor,
        const ALLEGRO_COLOR& fillColor,
        int score, BonusType bonus = BonusType::NONE);

  Vector vec;

  // Methods
  void destroy();
  void draw() const override;

  // Getters
  bool isDestroyed()   const;
  bool getSecondLife() const;
  int getScore()       const;
  Point getPosition()  const;
  float getWidth()     const;
  float getHeight()    const;
  BonusType getBonus() const;

  // Setters
  void setSecondLife(bool cas);
  void setFrameColor(const ALLEGRO_COLOR& color);
  void setBonus(BonusType bonusType);
};

#endif
