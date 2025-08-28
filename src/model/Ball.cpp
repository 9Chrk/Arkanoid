using namespace std;
#include "model/Ball.hpp"

/**
 * @brief Ball constructors
 */
Ball::Ball()
    : Circle({0, 0}, 0), direction(INITIAL_DIRECTION), speed(0),
      originalSpeed(0), moving(false), falling(false),
      catchActive(false), catchreleaseTimer(0) {}

Ball::Ball(float radius, float speed)
    : Circle({0, 0}, radius), direction(INITIAL_DIRECTION), speed(speed),
      originalSpeed(speed), moving(false), falling(false),
      catchActive(false), catchreleaseTimer(0) {}

/* ############## Movement functions ############## */

/**
 * @brief Place the ball above the spaceship when it is not moving
 */
void Ball::move(const Point& spaceship, float h) {
  position.x = spaceship.x;
  position.y = spaceship.y - h/2 - radius;
}

/**
 * @brief Move the ball while taking collisions into account
 */
void Ball::move(const Point& spaceship, float w, float h) {
  checkCollisions(spaceship, w, h);
  checkFall();
}

/* ############## Collision and state handling ############## */

/**
 * @brief Check collisions with the borders and the spaceship
 * Modify the ball's direction based on the impact angle with the spaceship
 */
void Ball::checkCollisions(const Point& spaceship, float w, float h) {
  Point newPos = {position.x + direction.x * speed, position.y + direction.y * speed};
  
  // Collisions with screen edges
  if (newPos.x - radius <= 0 || newPos.x + radius >= GAME_WIDTH) { direction.x *= -1; }
  if (newPos.y - radius <= 0) { direction.y *= -1; }
  
  // Collision with the spaceship
  if (newPos.y + radius >= spaceship.y - h/2 &&
    newPos.y + radius <= spaceship.y - h/2 + speed &&
    newPos.x >= spaceship.x - w/2 - radius &&
    newPos.x <= spaceship.x + w/2 + radius) {

      // Handle catch mode
      if (catchActive) {
          position = newPos;
          setMoving(false);
          return;
      }
      
      // Compute bounce angle based on impact position
      float x_rel = (newPos.x - spaceship.x) / (w/2); // relative position on the spaceship
      x_rel = clamp(x_rel, -1.0f, 1.0f);

      float alpha = BOUNCE_MIN_DEG + BOUNCE_RANGE_DEG * (1.0f - x_rel);
      alpha = clamp(alpha, BOUNCE_MIN_DEG, BOUNCE_MAX_DEG);                      // prevent perfectly vertical bounces
      float theta = alpha * static_cast<float>(M_PI) / 180.0f;
      
      // Update direction
      direction.x = cos(theta);
      direction.y = -sin(theta);
    }

    // Keep the ball within game boundaries
    newPos.x = clamp(newPos.x, radius, GAME_WIDTH - radius);
    newPos.y = clamp(newPos.y, radius, GAME_HEIGHT * OUT_OF_BOUNDS_Y_FACTOR);    // keep within game area
    position = newPos;
}
  
/**
 * @brief Check if the ball has fallen below the screen
 */
void Ball::checkFall() {
  falling = (position.y - radius > GAME_HEIGHT) ? true : false; // ball fell below the screen
}
  
/**
 * @brief Reset the direction and movement state
 */
void Ball::reset() {
  direction = INITIAL_DIRECTION;
  moving = false;
}

/**
 * @brief Manage the timer for catch mode
 */
void Ball::updateCatchReleaseTimer() {
    if (catchActive && !moving && catchreleaseTimer > 0.0f) {
        catchreleaseTimer -= TIMER_DECREMENT;
        if (catchreleaseTimer <= 0.0f) {
            setMoving(true);
            setCatchActive(false);
        }
    }
}

/**
 * @brief Gradually restore the original speed
 */
void Ball::updateSpeed() {
    if (speed < originalSpeed) {
        speed += originalSpeed * RESTORE_SPEED_RATE; // gradually restore speed
        if (speed > originalSpeed) {
            speed = originalSpeed;
        }
    }
}

// TODO
void Ball::startRestoreSpeed() {}
void Ball::stopRestoreSpeed() {}


/* ############## Accesseurs ############## */

// Getters
[[gnu::pure]] Point Ball::getDirection()     const { return direction; }
[[gnu::pure]] float Ball::getSpeed()         const { return speed;     }
[[gnu::pure]] bool  Ball::isMoving()         const { return moving;    }
[[gnu::pure]] bool  Ball::isFalling()        const { return falling;   }
[[gnu::pure]] bool  Ball::isCatchActive()    const { return catchActive; }
[[gnu::pure]] float Ball::getOriginalSpeed() const { return originalSpeed; }

// Setters
void Ball::setMoving(bool isMoving)   { moving = isMoving;   }
void Ball::setFalling(bool isFalling) { falling = isFalling; }
void Ball::setDirection(const Point& newDirection) { direction = newDirection; }
void Ball::setSpeed(float newSpeed) { speed = newSpeed; }
void Ball::setCatchActive(bool isActive) { catchActive = isActive;}
void Ball::setCatchReleaseTimer(float time) { catchreleaseTimer = time; }
void Ball::setOriginalSpeed() { speed = originalSpeed; };
