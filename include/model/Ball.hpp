#pragma once
#include "core/utils.hpp"
#include "core/Circle.hpp"


/**
 * @brief Ball handling movement and collisions
 *
 * Represents the ball in the Arkanoid game that bounces off walls,
 * the paddle, and the bricks. It also manages special effects such as
 * being caught by the paddle.
 */
class Ball : public Circle {
 private:
  // Movement properties
  Point direction;        // Movement direction (normalized vector)
  float speed;            // Current speed
  float originalSpeed;    // Original speed (for restoration)

  // Ball states
  bool  moving;           // In motion or stationary
  bool  falling;          // Falling out of the screen
  bool  catchActive;      // Catch mode enabled
  float catchreleaseTimer; // Timer to release the ball
  bool  restoringSpeed;   // Restoring speed

 public:
  /**
   * @brief Constructors
   */
  Ball();
  Ball(float radius, float speed);


  /* ############## Movement functions ############## */

  /// @brief Place the ball above the spaceship.
  /// @param spaceship Spaceship center.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float h);

  /// @brief Move the ball and handle collisions.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float w, float h);


  /* ######## Collision and state handling ######## */

  /// @brief Bounce off walls and the spaceship.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void checkCollisions(const Point& spaceship, float w, float h);

  /// @brief Detects when the ball leaves the screen.
  void checkFall();

  /// @brief Reset direction and movement.
  void reset();

  /**
   * @brief Timer and special effects management
   */
  void updateCatchReleaseTimer();  // Update the ball release timer
  void updateSpeed();              // Gradually update speed
  void startRestoreSpeed();        // Begin speed restoration
  void stopRestoreSpeed();         // Stop speed restoration

  /* ############## Accessors ############## */

  // Getters
  Point getDirection() const;
  float getSpeed()     const;
  bool isMoving()  const;
  bool isFalling() const;
  bool isCatchActive() const;
  float getOriginalSpeed() const;

  // Setters
  void setMoving(bool isMoving);
  void setFalling(bool isFalling);
  void setDirection(const Point& newDirection);
  void setSpeed(float newSpeed);
  void setCatchActive(bool active);
  void setCatchReleaseTimer(float time);
  void setOriginalSpeed(float origSpeed);
};
