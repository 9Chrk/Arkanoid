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
  vector<int> scores = {50, 90, 120, 100, 110, 80};
  vector<ALLEGRO_COLOR> colors = {GREY, RED, YELLOW, BLUE, MAGENTA, GREEN};
  const int dim_x = 13;
  const int dim_y = 6;
  
  const int width = 35;
  const int height = 15;
  const int margin = 3;
  
  const int offset_x = width + margin;
  const int offset_y = height + margin;
  const int total_width = dim_x * width + (dim_x - 1) * margin;
  const int offset_start_x = (500 - total_width) / 2;
  const int offset_start_y = 20;
  
  for (unsigned long i = 0; i < dim_x; ++i) {
    for (unsigned long j = 0; j < dim_y; ++j) {
      bricks.emplace_back(
        Point{offset_start_x + offset_x * static_cast<float>(i) + offset_x / 2.0f,
              offset_start_y + offset_y * static_cast<float>(j) + offset_y / 2.0f},
              width, height, BLACK, colors.at(j), scores.at(j));
    }
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
