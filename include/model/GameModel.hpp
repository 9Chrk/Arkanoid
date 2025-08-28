#pragma once
using namespace std;
#include "core/utils.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Bonus/Bonus.hpp"
#include "Spaceship.hpp"


/**
 * @brief Main model containing the game state and collision logic.
 *
 * This class centralizes Arkanoid's game logic by managing:
 * - The ball and its movement
 * - The player-controlled spaceship
 * - Bricks and their states
 * - Bonuses and their effects
 * - The score and the collision system
 */
class GameModel {
 private:
  /* ############## Game data ############## */
  json settings;                        // Game settings
  int score, highscore;                 // Current score and high score
  string levelFile;                     // Level file

  /* ############## Game entities ############## */
  Ball                                ball;       // Ball
  Spaceship                           spaceship;  // Spaceship
  vector<Brick>                       bricks;     // Bricks
  vector<shared_ptr<Bonus>>           bonuses;    // Active bonuses
  BonusType activeBonus = BonusType::NONE;        // Active bonus type

  /**
   * @brief Structure used to store collision results
   */
  struct CollisionResult {
    Brick* hitBrick;    // Hit brick
    int   hitSide;      // Side hit
    float distance;     // Collision distance

    CollisionResult() : hitBrick(nullptr), hitSide(-1), distance(INFINITY) {}
  };

  /* ############## Initialization methods ############## */
  void initializeBall();        // Initialize the ball
  void initializeSpaceship();   // Initialize the spaceship
  void initializeBricks();      // Initialize the bricks

  /* ############## Utility methods ############## */
  std::pair<std::string, std::string> splitAtSeparator(const std::string& str);

  
  /**
   * @brief Generate test points around the ball for collision detection
   * @param pos Ball center position
   * @return List of points to check
   */
  vector<Point> _collisionPoints(const Point& pos) const;


  /* ############## Collision detection ############## */

  /**
   * @brief Check whether collision search can be skipped
   * @param pos Current ball position
   * @param speed Movement speed
   * @return true if no collision is possible
   */
  bool shouldSkipCollisionCheck(const Point& pos, float speed) const;

  /**
   * @brief Find the first collision along the path
   * @param pos Current position
   * @param direction Normalized direction
   * @param speed Ball speed
   * @return Closest collision
   * @complexity O(N) over the number of bricks
   */
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);

  /**
   * @brief Test the intersection between a brick and the trajectory
   * @param brick Brick to test
   * @param collisionPoints Sampling points
   * @param direction Normalized direction
   * @param speed Ball speed
   * @return Potential collision with this brick
   */
  CollisionResult checkBrickCollision(Brick& brick,
                                      const vector<Point>& collisionPoints,
                                      const Point& direction,
                                      float speed) const;

  /**
   * @brief Update the score and brick state when an impact occurs
   * @param hitBrick Hit brick
   */
  void handleBrickHit(Brick* hitBrick);

  /**
   * @brief Create a bonus when a brick is destroyed
   * @param brick Destroyed brick
   */
  void spawnBonus(const Brick& brick);

  /**
   * @brief Apply bounce depending on the side hit
   * @param direction Ball direction to modify
   * @param hitSide Brick side (0 horizontal, 1 vertical, otherwise a corner)
   */
  void handleBallRebound(Point& direction, int hitSide);

 public:
  /**
   * @brief Constructor
   * @param levelFile Level file
   * @param score Initial score
   */
  GameModel(const string& levelFile, int score);

  /* ############## Game management methods ############## */

  /**
   * @brief Detect and handle current ball collisions
   * @complexity O(N) over the number of active bricks
   */
  void checkCollisions();

  /**
   * @brief Score management
   */
  void saveHighScore();     // Save the best score
  void resetHighScore();    // Reset the best score
  void resetScore();        // Reset the current score

  /**
   * @brief Bonus management
   */
  void updateBonuses();         // Update bonuses state
  void updateTimerBonuses();    // Update bonus timers
  BonusType getActiveBonus() const { return activeBonus; }
  void setActiveBonus(BonusType type) { activeBonus = type; }
  void clearActiveBonus();      // Remove active bonus effect

   
  /* ############## Utility methods ############## */

  /**
   * @brief Indicate whether the ball's direction has changed
   * @param tempDirection Previous direction, updated on output
   * @return true if a change occurred
   */
  bool checkDirectionChanged(Point& tempDirection) const;

  /* ############## Accessors ############## */

  // Simple getters
  int getScore()           const;
  int getHighScore()       const;
  Point getBallDirection() const;

  // Entity access
  Ball& getBall();
  const Ball& getBall() const;

  Spaceship& getSpaceship();
  const Spaceship& getSpaceship() const;

  const std::vector<Brick>&              getBricks()   const;
  const std::vector<shared_ptr<Bonus>>&  getBonuses() const;

  // Game state
  bool win()  const;  // Check if the player has won
  bool lose() const;  // Check if the player has lost
};
