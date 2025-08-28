#pragma once
#include "Bonus.hpp"


class Expand : public Bonus {
 public:
  Expand(const Point& position, float w, float h, float fallSpeed);
  ~Expand() override;

  void applyEffect(GameModel& model) override;
};
