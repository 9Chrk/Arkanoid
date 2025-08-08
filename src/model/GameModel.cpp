using namespace std;
#include "GameModel.hpp"


GameModel::GameModel(const string& levelFile, int score)
         : settings(loadSettings()), score(score), highscore(settings["highscore"].get<int>()),
           levelFile(levelFile), ball(), spaceship(), bricks(), bonuses() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
  // Bonus initialization
}

// Initialization methods

void GameModel::initializeBall() 
{
  json _ball                    = settings["ball"];
  float radius                  = _ball["radius"].get<float>();
  float speed                   = _ball["speed"].get<float>();
  ball                          = Ball(radius, speed);
}

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

  const float offset_x          = static_cast<float>(width + margin);
  const float offset_y          = static_cast<float>(height + margin);
  const float total_width       = static_cast<float>(dim_x * width + (dim_x - 1) * margin);
  const float offset_start_x    = (_offset_start_x > 0) ? static_cast<float>(_offset_start_x) : (GAME_WIDTH - total_width)/2.0f; // center the grid if no offset
  const float offset_start_y    = static_cast<float>(_offset_start_y);

  for (size_t index = 0; index < bricks_data.size(); ++index) {
    size_t i = index / static_cast<size_t>(dim_x);
    size_t j = index % static_cast<size_t>(dim_x);
    int score = stoi(bricks_data.at(index));

    if (score != EMPTY_BRICK) {
      bricks.emplace_back(
        Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x/2,
               offset_start_y + offset_y * static_cast<float>(i) + offset_y/2}),
        width, height, score, BonusType::NONE
      );
    }
  }
}

// Collision handling

void GameModel::checkCollisions() {
  Point pos = ball.getPosition();
  float speed = ball.getSpeed();
  Point direction = Vector::normalize(ball.getDirection());
  
  if (shouldSkipCollisionCheck(pos, speed)) return;                        // avoid unnecessary search
  CollisionResult collision = findClosestCollision(pos, direction, speed); // find the first collision

  if (!collision.hitBrick) return;                                         // no collision, move normally
  handleBrickHit(collision.hitBrick);                                      // update score
  handleBallRebound(direction, collision.hitSide);                         // handle bounce
}

bool GameModel::shouldSkipCollisionCheck(const Point& pos, float speed) const {
  const Brick &last = bricks.back();
  return pos.y >= last.getPosition().y + (last.getHeight()/2) + (speed * speed);
}

GameModel::CollisionResult GameModel::findClosestCollision(const Point& pos, const Point& direction, float speed) {
  const vector<Point> collisionPoints = _collisionPoints(pos);
  CollisionResult result;
  
  for (Brick &brick : bricks) {
    if (brick.isDestroyed()) continue;
    
    CollisionResult brickCollision = checkBrickCollision(brick, collisionPoints, direction, speed);
    if (brickCollision.hitBrick && brickCollision.distance < result.distance) {
      result = brickCollision;
    }
  }
  return result;
}

GameModel::CollisionResult GameModel::checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const {
  CollisionResult result;
  
  for (const Point &p : collisionPoints) {
    Point nextP = {p.x + direction.x * speed, p.y + direction.y * speed};
    pair<Point, int> intersectionResult = brick.getVector().intersection({p, nextP});
    int side = intersectionResult.second;
    
    if (side == -1) continue;
    
    float distance = Vector::distance(p, intersectionResult.first);
    if (distance < result.distance) {
      result.hitSide = side;
      result.hitBrick = &brick;
      result.distance = distance;
    }
  }
  return result;
}

void GameModel::handleBrickHit(Brick* hitBrick) {
  if (hitBrick->getScore() != SECOND_LIFE || !hitBrick->getSecondLife()) {
    if (hitBrick->getScore() != GOLD_BRICK) {
      hitBrick->destroy();
      score += hitBrick->getScore();
    }
  } else {
    hitBrick->setSecondLife(false);
  }
}

void GameModel::handleBallRebound(Point& direction, int hitSide) {
  if (hitSide == 1) {
    direction.x *= -1;
  } else if (hitSide == 0) {
    direction.y *= -1;
  } else {
    direction.x *= -1;
    direction.y *= -1;
  }
  ball.setDirection(direction);
}

// Utility methods

[[gnu::pure]] vector<Point> GameModel::_collisionPoints(const Point& pos) const {
  vector<Point> collisionPoints = {
    {pos.x, pos.y},
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}, // Top-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Left
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}  // Top-Left
  };
  return collisionPoints;
}

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

// Score management

void GameModel::saveHighScore() {
  if (score >= highscore) {
    highscore = score;
    writeJsonFile("./assets/data/settings.json", "highscore", highscore);
  }
}

void GameModel::resetHighScore() {
  highscore = 0;
  writeJsonFile("./assets/data/settings.json", "highscore", highscore);
}

void GameModel::resetScore() { 
  score = 0; 
}

// Getters

[[gnu::pure]] int GameModel::getScore()           const { return score; }
[[gnu::pure]] int GameModel::getHighScore()       const { return highscore; }
[[gnu::pure]] Point GameModel::getBallDirection() const { return ball.getDirection(); }

[[gnu::pure]] const Ball& GameModel::getBall()           const { return ball; }
[[gnu::pure]] const Spaceship& GameModel::getSpaceship() const { return spaceship; }

[[gnu::pure]] Ball& GameModel::getBall() { return ball; }
[[gnu::pure]] Spaceship& GameModel::getSpaceship() { return spaceship; }

[[gnu::pure]] const vector<Brick>& GameModel::getBricks()  const { return bricks;  }
[[gnu::pure]] const vector<Bonus>& GameModel::getBonuses() const { return bonuses; }

[[gnu::pure]] bool GameModel::lose() const { return spaceship.isDeath(); }

[[gnu::pure]] bool GameModel::win()  const {
  for (auto& brick : bricks) if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  return true;
}
