#include "model/Bonus/Split.hpp"
#include "model/GameModel.hpp"

Split::Split(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::SPLIT, fallSpeed) {}

Split::~Split() = default;

void Split::applyEffect(GameModel& model) {
  Ball& ball = model.getBall();
  Point dir = ball.getDirection();
  dir.x *= -1.0f; // simple approximation of a split
  ball.setDirection(dir);
}
