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
  vector<Point> _collisionPoints(Point pos);

  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

 public:
  Games(string levelFile, int score);

  Ball ball;
  Spaceship spaceship;
  void draw();
  bool win();
  bool loose();
  void checkCollisions();
  int getScore();
  int getHighScore();
  Point getLastCollisionPos();
  void setLastCollisionPos(Point newPos);
  void saveHighScore();
  void resetHighScore();
};

#endif
