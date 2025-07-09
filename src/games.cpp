#include "games.hpp"

Games::Games(string levelFile, int score) : score(score), levelFile(levelFile), lastCollisionPos({0, 0}) {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}

void Games::initializeBall() {
  json _ball = readJsonFile("./data/settings.json", "ball");
  float rayon = _ball["rayon"].get<float>();
  float vitesse = _ball["vitesse"].get<float>();
  ALLEGRO_COLOR color = getColor(_ball["color"].get<string>());
  
  ball = Ball(rayon, vitesse, color);
}

void Games::initializeSpaceship() {
  json _spaceship = readJsonFile("./data/settings.json", "spaceship");
  float position_x = _spaceship["position.x"].get<float>();
  float position_y = _spaceship["position.y"].get<float>();
  float width = _spaceship["width"].get<float>();
  float height = _spaceship["height"].get<float>();
  int vitesse = _spaceship["vitesse"].get<int>();
  int health = _spaceship["health"].get<int>();
  ALLEGRO_COLOR frameColor = getColor(_spaceship["frameColor"].get<string>());
  ALLEGRO_COLOR fillColor = getColor(_spaceship["fillColor"].get<string>());
  
  spaceship = Spaceship({position_x, position_y}, width, height, vitesse, health, frameColor, fillColor);
}

void Games::initializeBricks() {
  json level = openJsonFile("./data/" + levelFile);
  json settings = openJsonFile("./data/settings.json");

  vector<string> bricks_data = level["bricks"].get<vector<string>>();
  auto bricks_colors = settings["brick_colors"].get<map<string, string>>();

  const int dim_x = level["dim_x"].get<int>();

  const int width = settings["brick"]["width"].get<int>();
  const int height = settings["brick"]["height"].get<int>();
  const int margin = settings["brick"]["margin"].get<int>();

  const int _offset_start_x = settings["display_offset"]["offset_start_x"].get<int>();
  const int _offset_start_y = settings["display_offset"]["offset_start_y"].get<int>();

  const float offset_x = static_cast<float>(width + margin);
  const float offset_y = static_cast<float>(height + margin);
  const float total_width = static_cast<float>(dim_x * width + (dim_x - 1) * margin);
  const float offset_start_x = (_offset_start_x > 0) ? static_cast<float>(_offset_start_x) : (windowWidth - total_width) / 2.0f;
  const float offset_start_y = static_cast<float>(_offset_start_y);

  for (size_t index = 0; index < bricks_data.size(); ++index) {
      size_t i = index / static_cast<size_t>(dim_x);
      size_t j = index % static_cast<size_t>(dim_x);
    int score = stoi(bricks_data.at(index));
    if (score != -1) {
      bricks.emplace_back(
        Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x / 2.0f,
               offset_start_y + offset_y * static_cast<float>(i) + offset_y / 2.0f}),
        width, height, BLACK, getColor(bricks_colors.at(bricks_data.at(index))), score
      );
    }
  }
}

void Games::draw() {
  for (Brick brick : bricks) {
    if (!brick.isDestroyed()) {
      brick.draw();
    }
  }
  ball.draw();
  spaceship.draw();
}

vector<Point> Games::_collisionPoints(Point pos) {
  vector<Point> collisionPoints = {
    {pos.x, pos.y},
    {pos.x, pos.y - ball.getRayon()}, {pos.x, pos.y + ball.getRayon()},
    {pos.x - ball.getRayon(), pos.y}, {pos.x + ball.getRayon(), pos.y},
    {static_cast<float>(pos.x + ball.getRayon() / sqrt(2)), static_cast<float>(pos.y - ball.getRayon() / sqrt(2))}, // Haut-Droite
    {static_cast<float>(pos.x - ball.getRayon() / sqrt(2)), static_cast<float>(pos.y + ball.getRayon() / sqrt(2))}, // Bas-Gauche
    {static_cast<float>(pos.x + ball.getRayon() / sqrt(2)), static_cast<float>(pos.y + ball.getRayon() / sqrt(2))}  // Bas-Droite
  };
  return collisionPoints;
}

void Games::checkCollisions() {
  Point pos = ball.getPosition();
  float speed = ball.getVitesse();
  Point direction = ball.getDirection();

  Brick last_brick = bricks.at(bricks.size()-1);
  if (pos.y >= last_brick.getPosition().y + last_brick.getHeight()/2 + speed) return;

  vector<Point> collisionPoints = _collisionPoints(pos);

  for (Brick& brick : bricks) {
    for (auto& point : collisionPoints) {
      Point temp_pos = point;
      Point temp_next_pos = {pos.x + direction.x * speed * 1.5, pos.y + direction.y * speed * 1.5};
      int intersection =  brick.vec.intersection({temp_pos, temp_next_pos});
      if (!brick.isDestroyed() && intersection != -1) {
        if (brick.getScore() != 200 || (brick.getScore() == 200 && !brick.getSecondLife())) {
          if (brick.getScore() != 0) {
            brick.destroy();
            score += brick.getScore();
          }
          else if (brick.getPosition() == getLastCollisionPos()) { return; }
        } 
        else { brick.setSecondLife(false); }
        
        setLastCollisionPos(brick.getPosition());

        if (intersection) { direction.x *= -1; }  else { direction.y *= -1; }
        ball.setDirection(direction);
        return;
      }
    }
  }
}

void Games::saveHighScore() {
  json settings = openJsonFile("./data/settings.json");
  int highscore = settings["highscore"].get<int>();
  if (score >= highscore) { 
    writeJsonFile("./data/settings.json", "highscore", score);
  }
}

int Games::getHighScore() {
  return readJsonFile("./data/settings.json", "highscore").get<int>();
}

void Games::resetHighScore() {
  writeJsonFile("./data/settings.json", "highscore", 0);
}

bool Games::lose() { return spaceship.isDeath(); }

bool Games::win() {
  for (auto& brick : bricks) {
    if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  }
  return true;
}

void Games::setLastCollisionPos(Point newPos) { lastCollisionPos = newPos; }

[[gnu::pure]] int Games::getScore() { return score; }
[[gnu::pure]] Point Games::getLastCollisionPos() { return lastCollisionPos; }
