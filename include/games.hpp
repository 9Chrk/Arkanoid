#pragma once
#ifndef GAMES_HPP
#define GAMES_HPP

#include "common.hpp"
#include "brick.hpp"
#include "ball.hpp"
#include "spaceship.hpp"

struct CollisionResult {
  Brick* hitBrick;
  int hitSide;
  float distance;
  
  CollisionResult() : hitBrick(nullptr), hitSide(-1), distance(INFINITY) {}
};


// ################### Games class ###################

class Games {
 private:
  int score;
  string levelFile;
  vector<Brick> bricks;
  
  // Initialization methods
  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

  // Helper methods
  vector<Point> _collisionPoints(const Point& pos);
  bool shouldSkipCollisionCheck(const Point& pos, float speed);
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);
  CollisionResult checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed);
  void handleBrickHit(Brick* hitBrick);
  void handleBallRebound(Point& direction, int hitSide);

 public:
  Games(const string& levelFile, int score);

  Ball      ball;
  Spaceship spaceship;
  
  // Methods
  void draw() const;
  void checkCollisions();
  void saveHighScore()  const;
  void resetHighScore() const;
  void resetScore();

  // Getters
  int getScore()     const;
  int getHighScore() const;
  bool win()         const;
  bool lose()        const;
};

#endif
