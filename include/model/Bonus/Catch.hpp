#pragma once
#include "Bonus.hpp"

class Catch : public Bonus {
 public:
  Catch(const Point& position, float w, float h, float fallSpeed);
  ~Catch() override;

  void applyEffect(GameModel& model) override;
};
