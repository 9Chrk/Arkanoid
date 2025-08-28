#pragma once
#include "core/utils.hpp"

/**
 * @brief Class representing the player's spaceship
 *
 * The spaceship moves horizontally at the bottom of the screen and
 * allows the ball to bounce off it. It can receive various bonuses
 * that modify its properties (size, laser, etc.).
 */
class Spaceship : public Rectangle {
 private:
  // Spaceship properties
  int   health;         // Hit points
  float speed;          // Movement speed

  // States and bonuses
  bool  laserActive;    // Laser mode enabled
  float originalWidth;  // Original width (for restoration)
  float expandTimer;    // Timer for the expansion effect

 public:
  /**
   * @brief Constructors
   */
  Spaceship();
  Spaceship(const Point& position, float w, float h, float speed, int health);

  /* ############## Movement functions ############## */

  /**
   * @brief Move the spaceship in the given direction
   * @param dir Movement direction (left or right)
   */
  void move(Direction dir);

  /**
   * @brief Move the spaceship to the mouse position
   * @param mousePosition Current mouse position
   */
  void move(const Point& mousePosition);

  /* ############## State modification ############## */

  /**
   * @brief Set the spaceship width
   * @param newWidth New width
   */
  void setWidth(float newWidth);

  /**
   * @brief Enable or disable laser mode
   * @param enabled Laser state
   */
  void setLaser(bool enabled);

  /**
   * @brief Set the expansion timer
   * @param time Duration in seconds
   */
  void setExpandTimer(float time);

  /**
   * @brief Check whether a position is valid (within screen bounds)
   * @param position Position to check
   * @return true if the position is valid
   */
  bool validPosition(const Point& position) const;

  /**
   * @brief Manage spaceship health
   */
  void damage();    // Inflict one point of damage
  void addLife();   // Add one life

  /**
   * @brief Update the expansion timer
   * Restore the original size when the time runs out
   */
  void updateExpandTimer();

  /* ############## Accessors ############## */

  // Getters
  int getHealth() const;           // Remaining lives
  bool isDeath()  const;           // Whether the spaceship is destroyed
  bool hasLaser() const;           // Whether the laser is active
  float getSpeed() const;          // Movement speed
  float getExpandTimer() const;    // Remaining expansion time
  float getoriginalWidth() const;  // Original width
};
