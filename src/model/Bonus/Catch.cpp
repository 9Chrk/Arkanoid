#include "model/Bonus/Catch.hpp"
#include "model/GameModel.hpp"


Catch::Catch(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::CATCH, fallSpeed) {}

Catch::~Catch() = default;

void Catch::applyEffect(GameModel& model) {
  Ball& ball = model.getBall();
  Spaceship& ship = model.getSpaceship();
  ball.setCatchActive(true);
  ball.setCatchReleaseTimer(CATCH_RELEASE_DURATION);
}
