#include "games.hpp"

Games::Games() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}

void Games::initializeBall() {
  json _ball = readJsonFile("./data/settings.json", "ball");
  ball = Ball({_ball["position.x"].get<float>(), _ball["position.y"].get<float>()},
               _ball["rayon"].get<float>(), _ball["vitesse"].get<float>(), getColor(_ball["color"].get<string>()));
}

void Games::initializeSpaceship() {
  json _spaceship = readJsonFile("./data/settings.json", "spaceship");
  spaceship = Spaceship({_spaceship["position.x"].get<float>(), _spaceship["position.y"].get<float>()},
                         _spaceship["width"].get<float>(), _spaceship["height"].get<float>(),
                         _spaceship["vitesse"].get<int>(), _spaceship["health"].get<int>(),
                         getColor(_spaceship["frameColor"].get<string>()), getColor(_spaceship["fillColor"].get<string>()));
}

void Games::initializeBricks() {
  json level = openJsonFile("./data/level_1.json");
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
  const float offset_start_x = (_offset_start_x > 0) ? static_cast<float>(_offset_start_x) : (500 - total_width) / 2.0f;
  const float offset_start_y = static_cast<float>(_offset_start_y);

  for (size_t index = 0; index < bricks_data.size(); ++index) {
      size_t i = index / static_cast<size_t>(dim_x);
      size_t j = index % static_cast<size_t>(dim_x);
      bricks.emplace_back(
          Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x / 2.0f,
                offset_start_y + offset_y * static_cast<float>(i) + offset_y / 2.0f}),
          width, height, BLACK, getColor(bricks_colors.at(bricks_data.at(index))),
          stoi(bricks_data.at(index))
      );
  }
}

void Games::draw() {
  for (auto& brick : bricks) {
    if (!brick.isDestroyed()) {
      brick.draw();
    }
  }
  ball.draw();
  spaceship.draw();
}

void Games::checkCollisions() {
  Point pos = ball.getPosition();
  vector<Point> collisionPoints = {
    {pos.x, pos.y - ball.getRayon()}, {pos.x, pos.y + ball.getRayon()},
    {pos.x - ball.getRayon(), pos.y}, {pos.x + ball.getRayon(), pos.y},
    {static_cast<float>(pos.x + ball.getRayon() / sqrt(2)), static_cast<float>(pos.y - ball.getRayon() / sqrt(2))}, // Haut-Droite
    {static_cast<float>(pos.x - ball.getRayon() / sqrt(2)), static_cast<float>(pos.y + ball.getRayon() / sqrt(2))}, // Bas-Gauche
    {static_cast<float>(pos.x + ball.getRayon() / sqrt(2)), static_cast<float>(pos.y + ball.getRayon() / sqrt(2))}  // Bas-Droite
  };
  
  for (auto& brick : bricks) {
    for (auto& point : collisionPoints) {
      if (!brick.isDestroyed() && brick.contains(point)) {
        brick.destroy();
        Point direction = ball.getDirection();
        direction.y *= -1;
        ball.setDirection(direction);
        return;
      }
    }
  }
}

bool Games::loose() { return spaceship.isDeath(); }

bool Games::win() {
  for (auto& brick : bricks) {
    if (!brick.isDestroyed()) return false;
  }
  return true;
}
