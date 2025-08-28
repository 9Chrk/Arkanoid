#include "model/Bonus/Laser.hpp"
#include "model/GameModel.hpp"


Laser::Laser(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::LASER, fallSpeed) {}

Laser::~Laser() = default;

void Laser::applyEffect(GameModel& model) {
  model.getSpaceship().setLaser(true);
}
