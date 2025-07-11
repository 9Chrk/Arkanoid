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
  bool win()                  const;
  bool lose()                 const;

  // Other methods
  void draw() const;
  void updatePhysics(float dt);
  void saveHighScore() const;
  void resetHighScore() const;
};

#endif
