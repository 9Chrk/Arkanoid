#pragma once
#include "core/utils.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Spaceship.hpp"


/// @brief Model containing game state and collision logic.
class GameModel {
 private:
  json settings;
  int score, highscore;
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

  
  /* ############## Utility methods ############## */

  /// @brief Generate test points around the ball.
  /// @param pos Ball center position.
  /// @return List of points to check.
  vector<Point> _collisionPoints(const Point& pos) const;


  /* ############## Collision detection ############## */

  /// @brief Skip search if the ball is outside the useful area.
  /// @param pos Current ball position.
  /// @param speed Movement speed.
  /// @return true if no collision is possible.
  bool shouldSkipCollisionCheck(const Point& pos, float speed) const;

  /// @brief Find the first collision along the path.
  /// @param pos Current position.
  /// @param direction Normalized direction.
  /// @param speed Ball speed.
  /// @return Nearest collision.
  /// @complexity O(N) over the number of bricks.
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);

  /// @brief Test intersection between a brick and the trajectory.
  /// @param brick Brick to test.
  /// @param collisionPoints Sampling points.
  /// @param direction Normalized direction.
  /// @param speed Ball speed.
  /// @return Potential collision with this brick.
  CollisionResult checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const;

  /// @brief Update score and brick state upon hit.
  /// @param hitBrick Brick impacted.
  void handleBrickHit(Brick* hitBrick);

  /// @brief Apply rebound based on the side hit.
  /// @param direction Ball direction to modify.
  /// @param hitSide Brick side (0 horizontal, 1 vertical, else corner).
  void handleBallRebound(Point& direction, int hitSide);

 public:
  GameModel(const string& levelFile, int score);

  // Methods
  /// @brief Detect and handle the ball's current collision.
  /// @complexity O(N) over the number of active bricks.
  void checkCollisions();

  void saveHighScore();
  void resetHighScore();
  void resetScore();

   
  /* ############## Utility methods ############## */

  /// @brief Indicate if the ball's direction has changed.
  /// @param tempDirection Previous direction, updated on output.
  /// @return true if a change occurred.
  bool checkDirectionChanged(Point& tempDirection) const;

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
