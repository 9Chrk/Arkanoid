#pragma once
#ifndef BRICK_HPP
#define BRICK_HPP

#include "common.hpp"
#include "rectangle.hpp"
#include "vecteur.hpp"


class Brick : public Rectangle {
 private:
  int score;
  bool destroyed, secondLife;

 public:
  Brick(const Point& position, float w, float h,
        const ALLEGRO_COLOR& frameColor,
        const ALLEGRO_COLOR& fillColor,
        int score);

  Vecteur vec;

  // Actions
  void destroy();

  // Getters
  bool isDestroyed()   const;
  bool getSecondLife() const;
  int getScore()       const;
  Point getPosition()  const;
  float getWidth()     const;
  float getHeight()    const;

  // Setters
  void setSecondLife(bool cas);
};

#endif
