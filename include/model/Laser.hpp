#pragma once
#include "model/Bonus.hpp"

class Laser : public Bonus {
 public:
  Laser(const Point& position, float w, float h, float fallSpeed);
  ~Laser() override;

  void applyEffect(GameModel& model) override;
};
