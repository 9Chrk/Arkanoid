#pragma once
#include "core/utils.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Spaceship.hpp"


/// @brief Modèle contenant l'état du jeu et la logique de collision.
class GameModel {
 private:
  int score, highscore;
  string levelFile;

  Ball           ball;
  Spaceship      spaceship;
  vector<Brick>  bricks;
  vector<Bonus>  bonuses;

  struct CollisionResult {
    Brick* hitBrick;
    int hitSide;
    float distance;
  
    CollisionResult() : hitBrick(nullptr), hitSide(-1), distance(INFINITY) {}
  };
  
  // Méthodes d'initialisation
  void initializeBall();
  void initializeSpaceship();
  void initializeBricks();

  
  /* ############## Méthodes utilitaires ############## */

  /// @brief Génère les points de test autour de la balle.
  /// @param pos Position centrale de la balle.
  /// @return Liste de points à vérifier.
  vector<Point> _collisionPoints(const Point& pos) const;


  /* ############## Détection de collision ############## */
 
  /// @brief Ignore la recherche si la balle est hors zone utile.
  /// @param pos Position actuelle de la balle.
  /// @param speed Vitesse de déplacement.
  /// @return true si aucune collision n'est possible.
  bool shouldSkipCollisionCheck(const Point& pos, float speed) const;

  /// @brief Cherche la première collision sur la trajectoire.
  /// @param pos Position actuelle.
  /// @param direction Direction normalisée.
  /// @param speed Vitesse de la balle.
  /// @return Collision la plus proche.
  /// @complexity O(N) sur le nombre de briques.
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);

  /// @brief Teste l'intersection entre une brique et la trajectoire.
  /// @param brick Brique à tester.
  /// @param collisionPoints Points d'échantillonnage.
  /// @param direction Direction normalisée.
  /// @param speed Vitesse de la balle.
  /// @return Collision éventuelle avec cette brique.
  CollisionResult checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const;

  /// @brief Met à jour score et état de la brique touchée.
  /// @param hitBrick Brique impactée.
  void handleBrickHit(Brick* hitBrick);

  /// @brief Applique le rebond selon la face touchée.
  /// @param direction Direction de la balle à modifier.
  /// @param hitSide Côté de la brique (0 horizontal, 1 vertical, autre coin).
  void handleBallRebound(Point& direction, int hitSide);

 public:
  GameModel(const string& levelFile, int score);

  // Méthodes
  /// @brief Détecte et gère la collision courante de la balle.
  /// @complexity O(N) sur le nombre de briques actives.
  void checkCollisions();

  void saveHighScore();
  void resetHighScore();
  void resetScore();

   
  /* ############## Méthodes utilitaires ############## */
  
  /// @brief Indique si la direction de la balle a changé.
  /// @param tempDirection Direction précédente, mise à jour en sortie.
  /// @return true si un changement a eu lieu.
  bool checkDirectionChanged(Point& tempDirection) const;

  // Getters
  int getScore()           const;
  int getHighScore()       const;
  Point getBallDirection() const;

  Ball& getBall();
  const Ball& getBall() const;

  Spaceship& getSpaceship();
  const Spaceship& getSpaceship() const;

  const std::vector<Brick>& getBricks() const;
  const std::vector<Bonus>& getBonuses() const;

  bool win()  const;
  bool lose() const;
};
