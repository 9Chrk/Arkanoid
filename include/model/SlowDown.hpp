#pragma once
#include "model/Bonus.hpp"

class SlowDown : public Bonus {
 public:
  SlowDown(const Point& position, float w, float h, float fallSpeed);
  ~SlowDown() override;

  void applyEffect(GameModel& model) override;
};
