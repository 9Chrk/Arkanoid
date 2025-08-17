using namespace std;
#include "model/Bonus/Bonus.hpp"


Bonus::Bonus(const Point& position, float w, float h, BonusType type, float fallSpeed)
     : Rectangle(position, w, h), type(type), active(false), collected(false), fallSpeed(fallSpeed) {}

// Methods

void Bonus::update() {
  if (active) { position.y += fallSpeed; } // move downward when active
}

// Bonus type management

[[gnu::pure]] string Bonus::getAbbreviation(const BonusType& bonusType) {
  if      (bonusType == BonusType::CATCH)      return "CT";
  else if (bonusType == BonusType::EXPAND)     return "EX";
  else if (bonusType == BonusType::EXTRA_LIFE) return "EL";
  else if (bonusType == BonusType::LASER)      return "LS";
  else if (bonusType == BonusType::SLOW_DOWN)  return "SD";
  else if (bonusType == BonusType::SPLIT)      return "SP";
  else { return ""; }
}

[[gnu::pure]] BonusType Bonus::fromAbbreviation(const string& bonusName) {
  if      (bonusName == "CT") return BonusType::CATCH;
  else if (bonusName == "EX") return BonusType::EXPAND;
  else if (bonusName == "EL") return BonusType::EXTRA_LIFE;
  else if (bonusName == "LS") return BonusType::LASER;
  else if (bonusName == "SD") return BonusType::SLOW_DOWN;
  else if (bonusName == "SP") return BonusType::SPLIT;
  else { return BonusType::NONE; }
}

// Getters

[[gnu::pure]] bool      Bonus::isActive()    const { return active;    }
[[gnu::pure]] bool      Bonus::isCollected() const { return collected; }
[[gnu::pure]] float     Bonus::getSpeed()    const { return fallSpeed; }
[[gnu::pure]] BonusType Bonus::getType()     const { return type;      }

// Setters

void Bonus::setActive(bool isActive)     { active = isActive;     }
void Bonus::setCollected(bool hasBonus)  { collected = hasBonus;  }
void Bonus::setFallSpeed(float speed)    { fallSpeed = speed;     }
