#include "model/SlowDown.hpp"

SlowDown::SlowDown(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::SLOW_DOWN, fallSpeed) {}

SlowDown::~SlowDown() = default;

void SlowDown::applyEffect(GameModel& /*model*/) {
  // TODO: implement slowdown effect
}
