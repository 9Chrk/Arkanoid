#include "model/Split.hpp"

Split::Split(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::SPLIT, fallSpeed) {}

Split::~Split() = default;

void Split::applyEffect(GameModel& /*model*/) {
  // TODO: implement split effect
}
