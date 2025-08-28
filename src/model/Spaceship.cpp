using namespace std;
#include "model/Spaceship.hpp"

/* ############## Constructeurs ############## */

Spaceship::Spaceship()
    : Rectangle({0.0f, 0.0f}, 0, 0), health(0), speed(0), laserActive(false),
      originalWidth(0), expandTimer(0) {}

Spaceship::Spaceship(const Point& position, float w, float h, float speed, int health)
    : Rectangle(position, w, h), health(health), speed(speed), laserActive(false),
      originalWidth(w), expandTimer(0) {}

/* ############## Accesseurs ############## */

// Getters
[[gnu::pure]] int   Spaceship::getHealth()   const { return health; }
[[gnu::pure]] bool  Spaceship::isDeath()     const { return health <= 0; }
[[gnu::pure]] bool  Spaceship::hasLaser()    const { return laserActive; }
[[gnu::pure]] float Spaceship::getSpeed()    const { return speed; }
[[gnu::pure]] float Spaceship::getExpandTimer() const { return expandTimer; }
[[gnu::pure]] float Spaceship::getoriginalWidth() const { return originalWidth; }

// Setters
/**
 * @brief Modifie la largeur en s'assurant que le vaisseau reste dans les limites
 */
void Spaceship::setWidth(float newWidth) {
    w = newWidth;
    position.x = clamp(position.x, w/2, GAME_WIDTH - w/2);
}

void Spaceship::setExpandTimer(float time) { expandTimer = time; }
void Spaceship::setLaser(bool enabled)     { laserActive = enabled; }

/* ############## Fonctions de mouvement ############## */

/**
 * @brief Déplace le vaisseau à gauche ou à droite
 * Empêche le vaisseau de sortir des limites de l'écran
 */
void Spaceship::move(Direction dir) {
    Point newPos = position;
    newPos.x += (dir == Direction::Left ? -speed : speed);
    position.x = clamp(newPos.x, w/2, GAME_WIDTH - w/2);
}

/**
 * @brief Déplace le vaisseau à la position de la souris
 * Vérifie que la position est valide et dans la zone jouable
 */
void Spaceship::move(const Point& mousePosition) {
    Point newPos = position;
    newPos.x = mousePosition.x;
    if (validPosition(newPos) && mousePosition.y >= GAME_HEIGHT * PLAYABLE_RATIO) {
        position = newPos;
    }
}

/* ############## Gestion de l'état ############## */

/**
 * @brief Ajoute une vie au vaisseau
 */
void Spaceship::addLife() { health++; }

/**
 * @brief Diminue la vie du vaisseau
 */
void Spaceship::damage()  { health--; }

/**
 * @brief Met à jour le minuteur d'agrandissement
 * Restaure la taille d'origine quand le temps est écoulé
 */
void Spaceship::updateExpandTimer() {
    if (expandTimer > 0.0f) {
        expandTimer -= TIMER_DECREMENT;
        if (expandTimer <= 0.0f) {
            setWidth(originalWidth);
        }
    }
}

/**
 * @brief Vérifie si la position est valide (dans les limites de l'écran)
 */
[[gnu::pure]] bool Spaceship::validPosition(const Point& newPos) const {
    return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= GAME_WIDTH);
}
