#pragma once
#include "core/utils.hpp"
#include "Vector.hpp"
#include "Bonus/Bonus.hpp"


/// @brief Destructible brick that can contain a bonus.
class Brick : public Rectangle {
 private:
  int score;
  bool destroyed, secondLife;
  BonusType bonus;
 
  Vector vec;

 public:
  Brick(const Point& position, float w, float h, int score, BonusType bonus = BonusType::NONE);

  /// @brief Mark the brick as destroyed.
  void destroy();

  // Getters
  bool isDestroyed()   const;
  bool getSecondLife() const;
  int getScore()       const;
  BonusType getBonus() const;
  Vector getVector()   const;

  // Setters
  void setSecondLife(bool cas);
  void setBonus(BonusType bonusType);
};
