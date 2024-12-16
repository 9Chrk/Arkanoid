#pragma once
#ifndef GAMES_HPP
#define GAMES_HPP

#include "common.hpp"
#include "brick.hpp"
#include "ball.hpp"
#include "spaceship.hpp"

class Games {
 private:
  vector<Brick> bricks;
  int score;
  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

 public:
  Games();

  Ball ball;
  Spaceship spaceship;
  void draw();
  bool win();
  bool loose();
  void checkCollisions();
  int getScoreGame();
};

#endif
