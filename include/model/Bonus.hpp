#pragma once
#include "core/utils.hpp"


enum class BonusType {
  NONE,
  LASER,
  EXPAND,
  CATCH,
  SLOW_DOWN,
  SPLIT,
  EXTRA_LIFE,
};

class Bonus : public Rectangle {
 private:
  BonusType type;
  bool active;
  float fallSpeed;

 public:
  Bonus(const Point& position, float w, float h, BonusType type, float fallSpeed); 

  // Methods
  void update();

  // Bonus type management
  static std::string getAbbreviation(const BonusType& bonusName); 
  static BonusType fromAbbreviation(const std::string& bonusName);
  
  // Getters
  bool isActive()     const;
  BonusType getType() const;
  float getSpeed()    const;

  // Setters
  void setActive(bool active);
  void setFallSpeed(float speed);
};
