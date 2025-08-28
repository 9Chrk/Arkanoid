#include "model/Bonus/Split.hpp"
#include "model/GameModel.hpp"


Split::Split(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::SPLIT, fallSpeed) {}

Split::~Split() = default;

void Split::applyEffect(GameModel& model) {
  (void)model;
}
