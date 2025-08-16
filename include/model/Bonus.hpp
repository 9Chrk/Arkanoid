#pragma once
#include "core/utils.hpp"

class GameModel;

/// @brief Available types of bonuses.
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
 protected:
  BonusType type;
  bool active;
  bool collected;
  float fallSpeed;

 public:
  Bonus(const Point& position, float w, float h, BonusType type, float fallSpeed);
  virtual ~Bonus() = default;

  // Methods
  void update();
  virtual void applyEffect(GameModel& model) = 0;

  // Bonus type management
  static std::string getAbbreviation(const BonusType& bonusName);
  static BonusType fromAbbreviation(const std::string& bonusName);

  // Getters
  bool isActive()     const;
  bool isCollected()  const;
  BonusType getType() const;
  float getSpeed()    const;

  // Setters
  void setActive(bool active);
  void setCollected(bool collected);
  void setFallSpeed(float speed);
};
