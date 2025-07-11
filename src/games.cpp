#include "games.hpp"


Games::Games(const string& levelFile, int score)
     : score(score), levelFile(levelFile), bricks(), ball(), spaceship() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}

// Initialization methods

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

// Getters

[[gnu::pure]] int Games::getScore() const {
  return score;
}

[[gnu::pure]] int Games::getHighScore() const {
  return readJsonFile("./data/settings.json", "highscore").get<int>();
}


[[gnu::pure]] bool Games::lose() const {
  return spaceship.isDeath(); 
}

[[gnu::pure]] bool Games::win() const {
  for (auto& brick : bricks) {
    if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  }
  return true;
}


// Other methods

void Games::draw() const {
  for (Brick brick : bricks) {
    if (!brick.isDestroyed()) {
      brick.draw();
    }
  }
  ball.draw();
  spaceship.draw();
}

void Games::updatePhysics(float dt) {
  float remaining = dt;
  const float EPS = 1e-4f;

  Vec2 pos = {ball.getPosition().x, ball.getPosition().y};
  Vec2 vel = {ball.getDirection().x * ball.getVitesse(),
              ball.getDirection().y * ball.getVitesse()};

  while (remaining > 0.f) {
      float bestTOI = remaining;
      Brick* hitB   = nullptr;
      Vec2   hitN;

      for (Brick& b : bricks) {
          if (b.isDestroyed()) continue;
          Rect box{b.getPosition().x - b.getWidth()/2.f,
                   b.getPosition().y - b.getHeight()/2.f,
                   b.getWidth(), b.getHeight()};
          float toi; Vec2 n;
          if (sweepCircleAABB(pos, vel, ball.getRayon(), box,
                              remaining, toi, n))
              if (toi < bestTOI) { bestTOI = toi; hitB = &b; hitN = n; }
      }

      if (!hitB) {
          pos = pos + vel * remaining;
          break;
      }

      pos = pos + vel * (bestTOI - EPS);

      vel = reflect(vel, hitN);

      if (hitB->getScore() != 200 || (hitB->getScore() == 200 && !hitB->getSecondLife())) {
          if (hitB->getScore() != 0) {
              hitB->destroy();
              score += hitB->getScore();
          }
      } else {
          hitB->setSecondLife(false);
      }

      remaining -= bestTOI;
  }

  Point newDir = {normalize(vel).x, normalize(vel).y};
  ball.setDirection(newDir);
  ball.setPosition({pos.x, pos.y});
  ball.setMouvement(true);
}

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
