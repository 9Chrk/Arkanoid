#include "model/Bonus/ExtraLife.hpp"
#include "model/GameModel.hpp"

ExtraLife::ExtraLife(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::EXTRA_LIFE, fallSpeed) {}

ExtraLife::~ExtraLife() = default;

void ExtraLife::applyEffect(GameModel& model) {
  model.getSpaceship().addLife();
}
