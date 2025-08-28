#include "model/Bonus/SlowDown.hpp"
#include "model/GameModel.hpp"
#include "core/constants.hpp"


SlowDown::SlowDown(const Point& position, float w, float h, float fallSpeed)
     : Bonus(position, w, h, BonusType::SLOW_DOWN, fallSpeed) {}

SlowDown::~SlowDown() = default;

void SlowDown::applyEffect(GameModel& model) {
    Ball& ball = model.getBall();
    // First slow down the ball
    ball.setSpeed(ball.getSpeed() * SLOW_DOWN_FACTOR);
}
