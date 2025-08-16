#pragma once
#include "model/Bonus.hpp"

class ExtraLife : public Bonus {
 public:
  ExtraLife(const Point& position, float w, float h, float fallSpeed);
  ~ExtraLife() override;

  void applyEffect(GameModel& model) override;
};
