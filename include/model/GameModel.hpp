#pragma once
#include "model/utils.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Spaceship.hpp"


class GameModel {
 private:
  int score;
  string levelFile;

  Ball           ball;
  Spaceship      spaceship;
  vector<Brick>  bricks;
  vector<Bonus>  bonuses;

  struct CollisionResult {
    Brick* hitBrick;
    int hitSide;
    float distance;
  
    CollisionResult() : hitBrick(nullptr), hitSide(-1), distance(INFINITY) {}
  };
  
  // Initialization methods
  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

  // Helper methods
  vector<Point> _collisionPoints(const Point& pos);

  // Collision detection methods
  bool shouldSkipCollisionCheck(const Point& pos, float speed);
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);
  CollisionResult checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed);
  
  void handleBrickHit(Brick* hitBrick);
  void handleBallRebound(Point& direction, int hitSide);

 public:
  GameModel(const string& levelFile, int score);

  // Methods
  void checkCollisions();

  void saveHighScore()  const;
  void resetHighScore() const;
  void resetScore();

  // Helper methods
  bool checkDirectionChanged(Point& tempDirection);

  // Getters
  int getScore()           const;
  int getHighScore()       const;
  Point getBallDirection() const;

  Ball& getBall();
  const Ball& getBall() const;

  Spaceship& getSpaceship();
  const Spaceship& getSpaceship() const;

  const std::vector<Brick>& getBricks() const;
  const std::vector<Bonus>& getBonuses() const;


  bool win()  const;
  bool lose() const;
};
