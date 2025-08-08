using namespace std;
#include "Ball.hpp"


Ball::Ball()
    : Circle({0, 0}, 0), direction(INITIAL_DIRECTION), speed(0),
      moving(false), falling(false) {}

Ball::Ball(float radius, float speed)
    : Circle({0, 0}, radius), direction(INITIAL_DIRECTION), speed(speed),
      moving(false), falling(false) {}

// Movement

void Ball::move(const Point& spaceship, float h) {
  position.x = spaceship.x;
  position.y = spaceship.y - h/2 - radius;
}

void Ball::move(const Point& spaceship, float w, float h) {
  checkCollisions(spaceship, w, h);
  checkFall();
}

// Collision and state management

void Ball::checkCollisions(const Point& spaceship, float w, float h) {
  Point newPos = {position.x + direction.x * speed, position.y + direction.y * speed};
  
  if (newPos.x - radius <= 0 || newPos.x + radius >= GAME_WIDTH) { direction.x *= -1; }
  if (newPos.y - radius <= 0) { direction.y *= -1; }
  
  if (newPos.y + radius >= spaceship.y - h/2 &&
    newPos.y + radius <= spaceship.y - h/2 + speed &&
    newPos.x >= spaceship.x - w/2 - radius &&
    newPos.x <= spaceship.x + w/2 + radius) {
      
      float x_rel = (newPos.x - spaceship.x) / (w/2);      // relative position on the spaceship
      x_rel = clamp(x_rel, -1.0f, 1.0f);

      float alpha = 30.0f + 120.0f * (1.0f - x_rel);
      alpha = clamp(alpha, 30.0f, 150.0f);                 // prevent perfectly vertical bounces
      float theta = alpha * static_cast<float>(M_PI) / 180.0f;
      
      direction.x = cos(theta);
      direction.y = -sin(theta);
    }
    newPos.x = clamp(newPos.x, radius, GAME_WIDTH - radius);
    newPos.y = clamp(newPos.y, radius, GAME_HEIGHT * 1.5f); // keep within game area
    position = newPos;
}
  
void Ball::checkFall() {
  falling = (position.y - radius > GAME_HEIGHT) ? true : false; // ball fell below the screen
}
  
void Ball::reset() {
  direction = INITIAL_DIRECTION;
  moving = false;
}

// Getters

[[gnu::pure]] Point Ball::getDirection() const { return direction; }
[[gnu::pure]] float Ball::getSpeed()     const { return speed;     }
[[gnu::pure]] bool  Ball::inMouvement()  const { return moving;    }
[[gnu::pure]] bool  Ball::isFalling()    const { return falling;   }

// Setters

void Ball::setMoving(bool isMoving)   { moving = isMoving;   }
void Ball::setFalling(bool isFalling) { falling = isFalling; }
void Ball::setDirection(const Point& newDirection) { direction = newDirection; }
