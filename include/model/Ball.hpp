#pragma once
#include "core/utils.hpp"
#include "core/Circle.hpp"


/**
 * @brief Ball handling movement and collisions.
 *
 * Représente la balle dans le jeu Arkanoid qui rebondit sur les murs,
 * la raquette et les briques. Gère également les effets spéciaux comme
 * la capture par la raquette.
 */
class Ball : public Circle {
 private:
  // Propriétés de mouvement
  Point direction;       // Direction de déplacement (vecteur normalisé)
  float speed;           // Vitesse actuelle
  float originalSpeed;   // Vitesse d'origine (pour restauration)

  // États de la balle
  bool moving;             // En mouvement ou statique
  bool falling;            // Tombe hors de l'écran
  bool catchActive;        // Mode capture activé
  float catchreleaseTimer; // Minuteur pour relâcher la balle
  bool restoringSpeed;     // En cours de restauration de vitesse

 public:
  Ball();
  Ball(float radius, float speed);


  /* ############## Fonctions de mouvement ############## */

  /// @brief Place the ball above the spaceship.
  /// @param spaceship Spaceship center.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float h);

  /// @brief Move the ball and handle collisions.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void move(const Point& spaceship, float w, float h);


  /* ######## Gestion des collisions et états ######## */

  /// @brief Bounce off walls and the spaceship.
  /// @param spaceship Spaceship center.
  /// @param w Spaceship width.
  /// @param h Spaceship height.
  void checkCollisions(const Point& spaceship, float w, float h);

  /// @brief Detects when the ball leaves the screen.
  void checkFall();

  /// @brief Reset direction and movement.
  void reset();

  /**
   * @brief Gestion des timers et effets spéciaux
   */
  void updateCatchReleaseTimer();  // Met à jour le timer de relâchement de la balle
  void updateSpeed();              // Met à jour la vitesse progressivement
  void startRestoreSpeed();        // Commence la restauration de vitesse
  void stopRestoreSpeed();         // Arrête la restauration de vitesse


  /* ############## Accesseurs ############## */

  // Getters
  Point getDirection() const;
  float getSpeed()     const;
  bool isMoving()  const;
  bool isFalling() const;
  bool isCatchActive() const;
  float getOriginalSpeed() const;

  // Setters
  void setMoving(bool isMoving);
  void setFalling(bool isFalling);
  void setDirection(const Point& newDirection);
  void setSpeed(float newSpeed);
  void setCatchActive(bool active);
  void setCatchReleaseTimer(float time);
  void setOriginalSpeed();
};
