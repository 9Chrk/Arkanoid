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

 public:
  Ball ball;
  Spaceship spaceship;
  Games();
  void draw();
  bool win();
  bool loose();
  void checkCollisions();
};

#endif
