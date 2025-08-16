#pragma once
#include "core/utils.hpp"
#include "core/Circle.hpp"


/// @brief Ball handling movement and collisions.
class Ball : public Circle {
 private:
  Point direction;
  float speed;
  bool moving, falling;

 public:
  Ball();
  Ball(float radius, float speed);


  /* ############## Movement ############## */
  
  /// @brief Place the ball above the spaceship.
  /// @param spaceship Spaceship center.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float h);

  /// @brief Move the ball and handle collisions.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float w, float h);


  /* ######## Collision and state management ######## */
  
  /// @brief Bounce off walls and the spaceship.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void checkCollisions(const Point& spaceship, float w, float h);

  /// @brief Detects when the ball leaves the screen.
  void checkFall();

  /// @brief Reset direction and movement.
  void reset();

  // Getters
  Point getDirection() const;
  float getSpeed()     const;

  bool isMoving()  const;
  bool isFalling() const;

  // Setters
  void setMoving(bool isMoving);
  void setFalling(bool isFalling);
  void setDirection(const Point& newDirection);
  void setSpeed(float newSpeed);
};
