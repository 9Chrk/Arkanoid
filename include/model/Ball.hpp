#pragma once
#include "core/utils.hpp"
#include "core/Circle.hpp"


/// @brief Balle gérant mouvement et collisions.
class Ball : public Circle {
 private:
  Point direction;
  float speed;
  bool moving, falling;

 public:
  Ball();
  Ball(float radius, float speed);


  /* ############## Mouvement ############## */ 
  
  /// @brief Place la balle au-dessus du vaisseau.
  /// @param spaceship Centre du vaisseau.
  /// @param h Hauteur du vaisseau.
  void move(const Point& spaceship, float h);

  /// @brief Déplace la balle et gère les collisions.
  /// @param spaceship Centre du vaisseau.
  /// @param w Largeur du vaisseau.
  /// @param h Hauteur du vaisseau.
  void move(const Point& spaceship, float w, float h);


  /* ######## Gestion des collisions et de l'état ######## */
  
  /// @brief Rebonds sur murs et vaisseau.
  /// @param spaceship Centre du vaisseau.
  /// @param w Largeur du vaisseau.
  /// @param h Hauteur du vaisseau.
  void checkCollisions(const Point& spaceship, float w, float h);

  /// @brief Détecte la sortie de l'écran.
  void checkFall();

  /// @brief Réinitialise direction et mouvement.
  void reset();

  // Getters
  Point getDirection() const;
  float getSpeed()     const;

  bool inMouvement()   const;
  bool isFalling()     const;

  // Setters
  void setMoving(bool isMoving);
  void setFalling(bool isFalling);
  void setDirection(const Point& newDirection);
};
