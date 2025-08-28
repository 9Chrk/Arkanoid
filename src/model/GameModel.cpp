using namespace std;
#include "model/GameModel.hpp"
#include "model/Bonus/Laser.hpp"
#include "model/Bonus/Expand.hpp"
#include "model/Bonus/Catch.hpp"
#include "model/Bonus/SlowDown.hpp"
#include "model/Bonus/Split.hpp"
#include "model/Bonus/ExtraLife.hpp"


/* ############## Constructor and initialization ############## */

/**
 * @brief Main constructor of the game model
 * Initializes the game components from configuration files
 */
GameModel::GameModel(const string& levelFile, int score)
         : settings(loadSettings()), score(score), highscore(settings["highscore"].get<int>()),
           levelFile(levelFile), ball(), spaceship(), bricks(), bonuses() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}

/// @brief Initialize the ball with parameters from the configuration file
void GameModel::initializeBall()
{
  json _ball                    = settings["ball"];
  float radius                  = _ball["radius"].get<float>();
  float speed                   = _ball["speed"].get<float>();
  ball                          = Ball(radius, speed);
}

/// @brief Initialize the spaceship with parameters from the configuration file
void GameModel::initializeSpaceship()
{
  json _spaceship               = settings["spaceship"];
  float position_x              = _spaceship["position.x"].get<float>();
  float position_y              = _spaceship["position.y"].get<float>();
  float width                   = _spaceship["width"].get<float>();
  float height                  = _spaceship["height"].get<float>();
  float speed                   = _spaceship["speed"].get<float>();
  int health                    = _spaceship["health"].get<int>();
  spaceship                     = Spaceship({position_x, position_y}, width, height, speed, health);
}

/**
 * @brief Initialize bricks from the level file
 * Create the layout of bricks with their attributes (points, bonus)
 */
void GameModel::initializeBricks()
{
  json level                    = openJsonFile("./assets/data/" + levelFile);
  vector<string> bricks_data    = level["bricks"].get<vector<string>>();
  const int dim_x               = level["dim_x"].get<int>();
  const int width               = settings["brick"]["width"].get<int>();
  const int height              = settings["brick"]["height"].get<int>();
  const int margin              = settings["brick"]["margin"].get<int>();
  const int _offset_start_x     = settings["display_offset"]["offset_start_x"].get<int>();
  const int _offset_start_y     = settings["display_offset"]["offset_start_y"].get<int>();

  // Compute positions
    const float offset_x       = static_cast<float>(width + margin);
    const float offset_y       = static_cast<float>(height + margin);
    const float total_width    = static_cast<float>(dim_x * width + (dim_x - 1) * margin);
    // Center the grid if no offset
    const float offset_start_x =
        (_offset_start_x > 0) ? static_cast<float>(_offset_start_x)
                              : (GAME_WIDTH - total_width) / 2.0f;
    const float offset_start_y = static_cast<float>(_offset_start_y);

    // Create bricks
  for (size_t index = 0; index < bricks_data.size(); ++index) {
    size_t i = index / static_cast<size_t>(dim_x);
    size_t j = index % static_cast<size_t>(dim_x);
    
    pair<string, string> info = splitAtSeparator(bricks_data.at(index));
    int score = stoi(info.first);
    BonusType bonusType = (info.second.empty() ? BonusType::NONE : Bonus::fromAbbreviation(info.second));

    if (score != EMPTY_BRICK) {
      bricks.emplace_back(
        Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x/2,
               offset_start_y + offset_y * static_cast<float>(i) + offset_y/2}),
        width, height, score, bonusType
      );
    }
  }
}

/**
 * @brief Split a string at the '|' separator
 * Used to extract the score and bonus type from bricks
 */
pair<string, string> GameModel::splitAtSeparator(const string& str) {
  const size_t pos = str.find('|');
  if (pos == string::npos) return {str, "" };
  return {str.substr(0, pos), str.substr(pos + 1)};
}


/* ############## Collision system ############## */

/**
 * @brief Detect and handle ball collisions with bricks
 * This central function finds the first brick hit and applies the effects
 */
void GameModel::checkCollisions() {
  Point pos = ball.getPosition();
  float speed = ball.getSpeed();
  Point direction = Vector::normalize(ball.getDirection());
  
  // Optimization: skip search if no collision is possible
  if (shouldSkipCollisionCheck(pos, speed)) return;

  // Find the first collision along the ball's path
  CollisionResult collision = findClosestCollision(pos, direction, speed);

  // No collision detected
  if (!collision.hitBrick) return;

  // Handle the impact
  handleBrickHit(collision.hitBrick);              // Score and destruction
  handleBallRebound(direction, collision.hitSide); // Bounce
}

/**
 * @brief Check whether collision search can be skipped
 * Optimization that avoids unnecessary calculations when the ball is too low
 */
bool GameModel::shouldSkipCollisionCheck(const Point& pos, float speed) const {
  const Brick &last = bricks.back();
  return pos.y >= last.getPosition().y + (last.getHeight()/2) + (speed * speed);
}

/**
 * @brief Find the closest collision along the ball's trajectory
 * Iterates through all active bricks to find the first impact
 */
GameModel::CollisionResult GameModel::findClosestCollision(const Point& pos, const Point& direction, float speed) {
  const vector<Point> collisionPoints = _collisionPoints(pos);
  CollisionResult result;
  
  // Test collision for each brick
  for (Brick &brick : bricks) {
    if (brick.isDestroyed()) continue;
    
    CollisionResult brickCollision = checkBrickCollision(brick, collisionPoints, direction, speed);
    if (brickCollision.hitBrick && brickCollision.distance < result.distance) {
      result = brickCollision;
    }
  }
  return result;
}

/**
 * @brief Test collision between a brick and the ball's trajectory
 * Uses multiple test points around the ball for precise detection
 */
GameModel::CollisionResult GameModel::checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const {
  CollisionResult result;
  
  // Test each collision point
  for (const Point &p : collisionPoints) {
    Point nextP = {p.x + direction.x * speed, p.y + direction.y * speed};
    pair<Point, int> intersectionResult = brick.getVector().intersection({p, nextP});
    int side = intersectionResult.second;
    
    // No intersection
    if (side == -1) continue;
    
    // Find the closest collision
    float distance = Vector::distance(p, intersectionResult.first);
    if (distance < result.distance) {
      result.hitSide = side;
      result.hitBrick = &brick;
      result.distance = distance;
    }
  }
  return result;
}

/**
 * @brief Handle the impact with a brick
 * Update the score and destroy the brick if needed
 */
void GameModel::handleBrickHit(Brick* hitBrick) {
  // Process according to brick type
  if (hitBrick->getScore() != SECOND_LIFE || !hitBrick->getSecondLife()) {
    if (hitBrick->getScore() != GOLD_BRICK) {
      hitBrick->destroy();
      score += hitBrick->getScore();
      spawnBonus(*hitBrick);
    }
  } else {
    hitBrick->setSecondLife(false);
  }
}

/**
 * @brief Modify the ball's direction based on the side hit
 * Different bounces depending on whether the ball hits a horizontal side, vertical side, or a corner
 */
void GameModel::handleBallRebound(Point& direction, int hitSide) {
  if (hitSide == 1) {
    direction.x *= -1;      // Bounce on vertical side
  } else if (hitSide == 0) {
    direction.y *= -1;      // Bounce on horizontal side
  } else {
    direction.x *= -1;      // Bounce on corner (invert both directions)
    direction.y *= -1;
  }
  ball.setDirection(direction);
}

/**
 * @brief Create a bonus when a brick is destroyed
 * The bonus type depends on the brick attribute
 */
void GameModel::spawnBonus(const Brick& brick) {
  BonusType type = brick.getBonus();
  if (type == BonusType::NONE) return;

  // Bonus parameters
  json bonusSettings = settings["bonus"];
  float width  = bonusSettings["width"].get<float>();
  float height = bonusSettings["height"].get<float>();
  float speed  = bonusSettings["speed"].get<float>();
  Point position = brick.getPosition();

  // Create the bonus according to its type
  shared_ptr<Bonus> bonus;
  switch (type) {
    case BonusType::LASER:      bonus = make_shared<Laser>(position, width, height, speed); break;
    case BonusType::EXPAND:     bonus = make_shared<Expand>(position, width, height, speed); break;
    case BonusType::CATCH:      bonus = make_shared<Catch>(position, width, height, speed); break;
    case BonusType::SLOW_DOWN:  bonus = make_shared<SlowDown>(position, width, height, speed); break;
    case BonusType::SPLIT:      bonus = make_shared<Split>(position, width, height, speed); break;
    case BonusType::EXTRA_LIFE: bonus = make_shared<ExtraLife>(position, width, height, speed); break;
    default: return;
  }

  // Activate and add the bonus
  bonus->setActive(true);
  bonuses.push_back(bonus);
}


/* ############## Bonus management ############## */

/**
 * @brief Update all active bonuses and check collisions with the spaceship
 */
void GameModel::updateBonuses() {
  for (auto it = bonuses.begin(); it != bonuses.end(); ) {
    shared_ptr<Bonus> bonus = *it;
    if (bonus->isActive()) {
      bonus->update();
      if (bonus->intersects(spaceship)) {
        // Disable the previous bonus before applying the new one
        clearActiveBonus();

        // Apply the new bonus
        bonus->setCollected(true);
        bonus->applyEffect(*this);

        // Set this bonus as active
        setActiveBonus(bonus->getType());

        it = bonuses.erase(it);
        continue;
      }
    }
    ++it;
  }
}

/**
 * @brief Update timers of active bonuses
 */
void GameModel::updateTimerBonuses() {
  spaceship.updateExpandTimer();
  ball.updateCatchReleaseTimer();
  ball.updateSpeed();
}

/**
 * @brief Disable the current bonus and restore the normal state
 */
void GameModel::clearActiveBonus() {
  switch (activeBonus) {
    case BonusType::EXPAND:
      spaceship.setWidth(spaceship.getoriginalWidth());
      spaceship.setExpandTimer(0);
      break;
    case BonusType::CATCH:
      ball.setCatchActive(false);
      ball.setCatchReleaseTimer(0);
      break;
    case BonusType::SLOW_DOWN:
      ball.setSpeed(ball.getOriginalSpeed());
      break;
      // Add the other bonus types...
    default:
      break;
  }
  activeBonus = BonusType::NONE;
}


/* ############## Utility methods ############## */

/**
 * @brief Generate points around the ball for collision detection
 * Allows more precise detection by testing several points (center and edges)
 */
[[gnu::pure]] vector<Point> GameModel::_collisionPoints(const Point& pos) const {
  vector<Point> collisionPoints = {
    {pos.x, pos.y},
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)),
     static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}, // Top-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)),
     static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Left
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)),
     static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)),
     static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}  // Top-Left
  };
  return collisionPoints;
}

/**
 * @brief Check whether the ball's direction has changed
 * Used to optimize graphical rendering
 */
bool GameModel::checkDirectionChanged(Point& tempDirection) const {
  const Point& currentDirection = ball.getDirection();
  constexpr float eps = numeric_limits<float>::epsilon(); // floating-point tolerance
  if (fabs(currentDirection.x - tempDirection.x) > eps ||
      fabs(currentDirection.y - tempDirection.y) > eps) {
    tempDirection = currentDirection;
    return true;
  }
  return false;
}


/* ############## Score management ############## */

/// @brief Save the high score if the current score is higher
void GameModel::saveHighScore() {
  if (score >= highscore) {
    highscore = score;
    writeJsonFile("./assets/data/settings.json", "highscore", highscore);
  }
}

/// @brief Reset the high score
void GameModel::resetHighScore() {
  highscore = 0;
  writeJsonFile("./assets/data/settings.json", "highscore", highscore);
}

/// @brief Reset the current score
void GameModel::resetScore() {
  score = 0; 
}


/* ############## Accessors ############## */

// Getters
[[gnu::pure]] int GameModel::getScore()           const { return score; }
[[gnu::pure]] int GameModel::getHighScore()       const { return highscore; }
[[gnu::pure]] Point GameModel::getBallDirection() const { return ball.getDirection(); }

// Entity access
[[gnu::pure]] const Ball& GameModel::getBall()           const { return ball; }
[[gnu::pure]] const Spaceship& GameModel::getSpaceship() const { return spaceship; }

[[gnu::pure]] Ball& GameModel::getBall() { return ball; }
[[gnu::pure]] Spaceship& GameModel::getSpaceship() { return spaceship; }

[[gnu::pure]] const vector<Brick>& GameModel::getBricks()  const { return bricks;  }
[[gnu::pure]] const vector<shared_ptr<Bonus>>& GameModel::getBonuses() const { return bonuses; }

// Game state
[[gnu::pure]] bool GameModel::lose() const { return spaceship.isDeath(); }

[[gnu::pure]] bool GameModel::win()  const {
  for (auto& brick : bricks) if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  return true;
}
