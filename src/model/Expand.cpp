#include "model/Expand.hpp"

Expand::Expand(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::EXPAND, fallSpeed) {}

Expand::~Expand() = default;

void Expand::applyEffect(GameModel& /*model*/) {
  // TODO: implement expand effect
}
