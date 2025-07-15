#include "games.hpp"


Games::Games(const string& levelFile, int score)
     : score(score), levelFile(levelFile), bricks(), ball(), spaceship() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}


// ###################  Initialization methods  ###################

void Games::initializeBall() 
{
  json _ball                    = readJsonFile("./data/settings.json", "ball");
  float radius                  = _ball["radius"].get<float>();
  float speed                   = _ball["speed"].get<float>();
  ALLEGRO_COLOR color           = getColor(_ball["color"].get<string>());
  ball                          = Ball(radius, speed, color);
}

void Games::initializeSpaceship() 
{
  json _spaceship               = readJsonFile("./data/settings.json", "spaceship");
  float position_x              = _spaceship["position.x"].get<float>();
  float position_y              = _spaceship["position.y"].get<float>();
  float width                   = _spaceship["width"].get<float>();
  float height                  = _spaceship["height"].get<float>();
  int speed                     = _spaceship["speed"].get<int>();
  int health                    = _spaceship["health"].get<int>();
  ALLEGRO_COLOR frameColor      = getColor(_spaceship["frameColor"].get<string>());
  ALLEGRO_COLOR fillColor       = getColor(_spaceship["fillColor"].get<string>());
  spaceship                     = Spaceship({position_x, position_y}, width, height, speed, health, frameColor, fillColor);
}

void Games::initializeBricks() 
{
  json level                    = openJsonFile("./data/" + levelFile);
  json settings                 = openJsonFile("./data/settings.json");
  vector<string> bricks_data    = level["bricks"].get<vector<string>>();
  auto bricks_colors            = settings["brick_colors"].get<map<string, string>>();
  const int dim_x               = level["dim_x"].get<int>();
  const int width               = settings["brick"]["width"].get<int>();
  const int height              = settings["brick"]["height"].get<int>();
  const int margin              = settings["brick"]["margin"].get<int>();
  const int _offset_start_x     = settings["display_offset"]["offset_start_x"].get<int>();
  const int _offset_start_y     = settings["display_offset"]["offset_start_y"].get<int>();

  const float offset_x          = static_cast<float>(width + margin);
  const float offset_y          = static_cast<float>(height + margin);
  const float total_width       = static_cast<float>(dim_x * width + (dim_x - 1) * margin);
  const float offset_start_x    = (_offset_start_x > 0) ? static_cast<float>(_offset_start_x) : (windowWidth - total_width)/2.0f;
  const float offset_start_y    = static_cast<float>(_offset_start_y);


  for (size_t index = 0; index < bricks_data.size(); ++index) {
    size_t i = index / static_cast<size_t>(dim_x);
    size_t j = index % static_cast<size_t>(dim_x);
    int score = stoi(bricks_data.at(index));

    if (score != -1) {
      bricks.emplace_back(
        Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x/2.0f,
               offset_start_y + offset_y * static_cast<float>(i) + offset_y/2.0f}),
        width, height, BLACK, getColor(bricks_colors.at(bricks_data.at(index))), score
      );
    }
  }
}


// ###################  Methods  ###################

void Games::draw() const {
  for (Brick brick : bricks) {
    if (!brick.isDestroyed()) {
      brick.draw();
    }
  }
  ball.draw();
  spaceship.draw();
}

void Games::resetScore() { 
  score = 0; 
} 


// ###################  Collision management  ###################

void Games::checkCollisions() {
  Point pos = ball.getPosition();
  float speed = ball.getSpeed();
  Point direction = Vector::normalize(ball.getDirection());
  
  if (shouldSkipCollisionCheck(pos, speed)) return;                        // Quick check if the ball is too far away
  CollisionResult collision = findClosestCollision(pos, direction, speed); // Collision search
  
  if (!collision.hitBrick) return;                                         // No collision found, just move the ball
  handleBrickHit(collision.hitBrick);                                      // Score and destruction management
  handleBallRebound(direction, collision.hitSide);                         // Bounce management
}

bool Games::shouldSkipCollisionCheck(const Point& pos, float speed) {
  const Brick &last = bricks.back();
  return pos.y >= last.getPosition().y + (last.getHeight()/2) + (speed * speed);
}

CollisionResult Games::findClosestCollision(const Point& pos, const Point& direction, float speed) {
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

CollisionResult Games::checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) {
  CollisionResult result;
  
  for (const Point &p : collisionPoints) {
    Point nextP = {p.x + direction.x * speed, p.y + direction.y * speed};
    pair<Point, int> intersectionResult = brick.vec.intersection({p, nextP});
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

void Games::handleBrickHit(Brick* hitBrick) {
  if (hitBrick->getScore() != 200 || !hitBrick->getSecondLife()) {
    if (hitBrick->getScore() != 0) {
      hitBrick->destroy();
      score += hitBrick->getScore();
    }
  } else {
    hitBrick->setFrameColor(RED);
    hitBrick->setSecondLife(false);
  }
}

void Games::handleBallRebound(Point& direction, int hitSide) {
  if (hitSide == 1) {
    direction.x *= -1.f;
  } else if (hitSide == 0) {
    direction.y *= -1.f;
  } else {
    direction.x *= -1.f;
    direction.y *= -1.f;
  }
  ball.setDirection(direction);
}


// ###################  Helper methods  ###################

[[gnu::pure]] vector<Point> Games::_collisionPoints(const Point& pos) {
  vector<Point> collisionPoints = {
    {pos.x, pos.y},
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}, // Haut-Droite
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bas-Gauche
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bas-Droite
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}  // Haut-Gauche
  };
  return collisionPoints;
}


// ###################  File management  ###################

void Games::saveHighScore() const {
  json settings = openJsonFile("./data/settings.json");
  int highscore = settings["highscore"].get<int>();
  if (score >= highscore) { 
    writeJsonFile("./data/settings.json", "highscore", score);
  }
}

void Games::resetHighScore() const {
  writeJsonFile("./data/settings.json", "highscore", 0);
}


// ###################  Getters  ###################

[[gnu::pure]] bool Games::lose()         const { return spaceship.isDeath();                                          }
[[gnu::pure]] int  Games::getScore()     const { return score;                                                        }
[[gnu::pure]] int  Games::getHighScore() const { return readJsonFile("./data/settings.json", "highscore").get<int>(); }

[[gnu::pure]] bool Games::win() const {
  for (auto& brick : bricks) if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  return true;
}
