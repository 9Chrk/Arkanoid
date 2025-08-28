#include "model/Bonus/Expand.hpp"
#include "model/GameModel.hpp"

Expand::Expand(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::EXPAND, fallSpeed) {}

Expand::~Expand() = default;

void Expand::applyEffect(GameModel& model) {
  Spaceship& ship = model.getSpaceship();
  ship.setWidth(ship.getWidth() * EXPAND_WIDTH_FACTOR);
  ship.setExpandTimer(EXPAND_TIMER_DURATION);
}
