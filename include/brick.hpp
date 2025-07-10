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
  void destroy();
  [[nodiscard]] bool isDestroyed() const;
  [[nodiscard]] bool getSecondLife() const;
  void setSecondLife(bool cas);
  [[nodiscard]] int getScore() const;
  [[nodiscard]] Point getPosition() const;
  [[nodiscard]] float getWidth() const;
  [[nodiscard]] float getHeight() const;
};

#endif
