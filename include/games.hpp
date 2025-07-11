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

  // Initialization methods
  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

 public:
  Games(const string& levelFile, int score);

  Ball ball;
  Spaceship spaceship;

  // Getters
  int getScore()              const;
  int getHighScore()          const;
  Point getLastCollisionPos() const;
  bool win()                  const;
  bool lose()                 const;

  // Setters
  void setLastCollisionPos(const Point& newPos);

  // Other methods
  void draw() const;
  void checkCollisions();
  void saveHighScore() const;
  void resetHighScore() const;
};;

#endif
