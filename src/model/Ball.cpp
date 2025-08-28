using namespace std;
#include "model/Ball.hpp"


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

      if (catchActive) {
          position = newPos;
          setMoving(false);
          setCatchActive(false);
          return;
      }
      
      float x_rel = (newPos.x - spaceship.x) / (w/2); // relative position on the spaceship
      x_rel = clamp(x_rel, -1.0f, 1.0f);

      float alpha = BOUNCE_MIN_DEG + BOUNCE_RANGE_DEG * (1.0f - x_rel);
      alpha = clamp(alpha, BOUNCE_MIN_DEG, BOUNCE_MAX_DEG);                      // prevent perfectly vertical bounces
      float theta = alpha * static_cast<float>(M_PI) / 180.0f;
      
      direction.x = cos(theta);
      direction.y = -sin(theta);
    }
    newPos.x = clamp(newPos.x, radius, GAME_WIDTH - radius);
    newPos.y = clamp(newPos.y, radius, GAME_HEIGHT * OUT_OF_BOUNDS_Y_FACTOR);    // keep within game area
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
[[gnu::pure]] bool  Ball::isMoving()     const { return moving;    }
[[gnu::pure]] bool  Ball::isFalling()    const { return falling;   }
[[gnu::pure]] bool  Ball::isCatchActive() const { return catchActive; }

// Setters

void Ball::setMoving(bool isMoving)   { moving = isMoving;   }
void Ball::setFalling(bool isFalling) { falling = isFalling; }
void Ball::setDirection(const Point& newDirection) { direction = newDirection; }
void Ball::setSpeed(float newSpeed) { speed = newSpeed; }
void Ball::setCatchActive(bool isActive) { catchActive = isActive;}
