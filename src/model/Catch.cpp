#include "model/Catch.hpp"

Catch::Catch(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::CATCH, fallSpeed) {}

Catch::~Catch() = default;

void Catch::applyEffect(GameModel& /*model*/) {
  // TODO: implement catch effect
}
