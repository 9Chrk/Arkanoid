#pragma once
#include "core/utils.hpp"


/**
 * @brief Classe représentant le vaisseau contrôlé par le joueur
 *
 * Le vaisseau se déplace horizontalement en bas de l'écran et permet
 * de faire rebondir la balle. Il peut recevoir divers bonus modifiant
 * ses caractéristiques (taille, laser, etc.).
 */
class Spaceship : public Rectangle {
 private:
  // Propriétés du vaisseau
  int health;            // Points de vie
  float speed;           // Vitesse de déplacement

  // États et bonus
  bool laserActive;      // Mode laser activé
  float originalWidth;   // Largeur d'origine (pour restauration)
  float expandTimer;     // Minuteur pour l'effet d'agrandissement

 public:
  Spaceship();
  Spaceship(const Point& position, float w, float h, float speed, int health);


  /* ############## Fonctions de mouvement ############## */

  /**
   * @brief Déplace le vaisseau dans la direction indiquée
   * @param dir Direction du mouvement (gauche ou droite)
   */
  void move(Direction dir);

  /**
   * @brief Déplace le vaisseau à la position de la souris
   * @param mousePosition Position actuelle de la souris
   */
  void move(const Point& mousePosition);


  /* ############## Modification d'état ############## */

  /**
   * @brief Définit la largeur du vaisseau
   * @param newWidth Nouvelle largeur
   */
  void setWidth(float newWidth);

  /**
   * @brief Active ou désactive le mode laser
   * @param enabled État du laser
   */
  void setLaser(bool enabled);

  /**
   * @brief Définit le minuteur d'agrandissement
   * @param time Durée en secondes
   */
  void setExpandTimer(float time);
  
  /**
   * @brief Vérifie si la position est valide (dans les limites de l'écran)
   * @param position Position à vérifier
   * @return true si la position est valide
   */
  bool validPosition(const Point& position) const;

  /**
   * @brief Gestion de la vie du vaisseau
   */
  void damage();    // Inflige un point de dégât
  void addLife();   // Ajoute un point de vie

  /**
   * @brief Met à jour le minuteur d'agrandissement
   * Restaure la taille d'origine quand le temps est écoulé
   */
  void updateExpandTimer();

  
  /* ############## Accesseurs ############## */

  // Getters
  int getHealth() const;           // Nombre de vies restantes
  bool isDeath()  const;           // Vérifie si le vaisseau est détruit
  bool hasLaser() const;           // Vérifie si le laser est actif
  float getSpeed() const;          // Vitesse de déplacement
  float getExpandTimer() const;    // Temps restant d'agrandissement
  float getoriginalWidth() const;  // Largeur d'origine
};
