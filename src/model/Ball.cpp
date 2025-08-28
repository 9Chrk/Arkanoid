using namespace std;
#include "model/Ball.hpp"

/**
 * @brief Constructeurs de la balle
 */
Ball::Ball()
    : Circle({0, 0}, 0), direction(INITIAL_DIRECTION), speed(0),
      originalSpeed(0), moving(false), falling(false),
      catchActive(false), catchreleaseTimer(0) {}

Ball::Ball(float radius, float speed)
    : Circle({0, 0}, radius), direction(INITIAL_DIRECTION), speed(speed),
      originalSpeed(speed), moving(false), falling(false),
      catchActive(false), catchreleaseTimer(0) {}

/* ############## Fonctions de mouvement ############## */

/**
 * @brief Place la balle au-dessus du vaisseau quand elle n'est pas en mouvement
 */
void Ball::move(const Point& spaceship, float h) {
  position.x = spaceship.x;
  position.y = spaceship.y - h/2 - radius;
}

/**
 * @brief Déplace la balle en tenant compte des collisions
 */
void Ball::move(const Point& spaceship, float w, float h) {
  checkCollisions(spaceship, w, h);
  checkFall();
}

/* ############## Gestion des collisions et états ############## */

/**
 * @brief Vérifie les collisions avec les bords et le vaisseau
 * Modifie la direction de la balle selon l'angle d'impact avec le vaisseau
 */
void Ball::checkCollisions(const Point& spaceship, float w, float h) {
  Point newPos = {position.x + direction.x * speed, position.y + direction.y * speed};
  
  // Collisions avec les bords de l'écran
  if (newPos.x - radius <= 0 || newPos.x + radius >= GAME_WIDTH) { direction.x *= -1; }
  if (newPos.y - radius <= 0) { direction.y *= -1; }
  
  // Collision avec le vaisseau
  if (newPos.y + radius >= spaceship.y - h/2 &&
    newPos.y + radius <= spaceship.y - h/2 + speed &&
    newPos.x >= spaceship.x - w/2 - radius &&
    newPos.x <= spaceship.x + w/2 + radius) {

      // Gestion du mode capture
      if (catchActive) {
          position = newPos;
          setMoving(false);
          return;
      }
      
      // Calcul de l'angle de rebond selon la position d'impact
      float x_rel = (newPos.x - spaceship.x) / (w/2); // relative position on the spaceship
      x_rel = clamp(x_rel, -1.0f, 1.0f);

      float alpha = BOUNCE_MIN_DEG + BOUNCE_RANGE_DEG * (1.0f - x_rel);
      alpha = clamp(alpha, BOUNCE_MIN_DEG, BOUNCE_MAX_DEG);                      // prevent perfectly vertical bounces
      float theta = alpha * static_cast<float>(M_PI) / 180.0f;
      
      // Mise à jour de la direction
      direction.x = cos(theta);
      direction.y = -sin(theta);
    }

    // Maintien de la balle dans les limites du jeu
    newPos.x = clamp(newPos.x, radius, GAME_WIDTH - radius);
    newPos.y = clamp(newPos.y, radius, GAME_HEIGHT * OUT_OF_BOUNDS_Y_FACTOR);    // keep within game area
    position = newPos;
}
  
/**
 * @brief Vérifie si la balle est tombée sous l'écran
 */
void Ball::checkFall() {
  falling = (position.y - radius > GAME_HEIGHT) ? true : false; // ball fell below the screen
}
  
/**
 * @brief Réinitialise la direction et l'état de mouvement
 */
void Ball::reset() {
  direction = INITIAL_DIRECTION;
  moving = false;
}

/**
 * @brief Gestion du minuteur pour le mode capture
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
 * @brief Restaure progressivement la vitesse d'origine
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
