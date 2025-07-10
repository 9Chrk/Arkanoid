#pragma once
#ifndef GAMES_HPP
#define GAMES_HPP

#include "common.hpp"
#include "brick.hpp"
#include "ball.hpp"
#include "spaceship.hpp"

class Games {
 private:
  int score;
  string levelFile;
  vector<Brick> bricks;
  Point lastCollisionPos;
  vector<Point> _collisionPoints(const Point& pos);

  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

 public:
  Games(const string& levelFile, int score);

  Ball ball;
  Spaceship spaceship;
  void draw() const;
  [[nodiscard]] bool win() const;
  [[nodiscard]] bool lose() const;
  void checkCollisions();
  [[nodiscard]] int getScore() const;
  [[nodiscard]] int getHighScore() const;
  [[nodiscard]] Point getLastCollisionPos() const;
  void setLastCollisionPos(const Point& newPos);
  void saveHighScore() const;
  void resetHighScore() const;
};

#endif
