#pragma once
#include "model/Bonus.hpp"

class Split : public Bonus {
 public:
  Split(const Point& position, float w, float h, float fallSpeed);
  ~Split() override;

  void applyEffect(GameModel& model) override;
};
