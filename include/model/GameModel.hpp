#pragma once
using namespace std;
#include "core/utils.hpp"
#include "Brick.hpp"
#include "Ball.hpp"
#include "Bonus/Bonus.hpp"
#include "Spaceship.hpp"


/**
 * @brief Modèle principal contenant l'état du jeu et la logique de collision.
 *
 * Cette classe centralise la logique du jeu Arkanoid en gérant:
 * - La balle et son mouvement
 * - Le vaisseau contrôlé par le joueur
 * - Les briques et leurs états
 * - Les bonus et leurs effets
 * - Le score et le système de collision
 */
class GameModel {
 private:
  /* ############## Données du jeu ############## */
  json settings;                        // Paramètres du jeu
  int score, highscore;                 // Score actuel et meilleur score
  string levelFile;                     // Fichier de niveau

  /* ############## Entités du jeu ############## */
  Ball                                ball;       // Balle
  Spaceship                           spaceship;  // Vaisseau
  vector<Brick>                       bricks;     // Briques
  vector<shared_ptr<Bonus>>           bonuses;    // Bonus actifs
  BonusType activeBonus = BonusType::NONE;        // Type de bonus actif

  /**
   * @brief Structure pour stocker les résultats de collision
   */
  struct CollisionResult {
    Brick* hitBrick;    // Brique touchée
    int hitSide;        // Côté touché
    float distance;     // Distance de collision

    CollisionResult() : hitBrick(nullptr), hitSide(-1), distance(INFINITY) {}
  };
  
  /* ############## Méthodes d'initialisation ############## */
  void initializeBall();        // Initialise la balle
  void initializeSpaceship();   // Initialise le vaisseau
  void initializeBricks();      // Initialise les briques

  /* ############## Méthodes utilitaires ############## */
  std::pair<std::string, std::string> splitAtSeparator(const std::string& str);

  
  /**
   * @brief Génère des points de test autour de la balle pour la détection de collision
   * @param pos Position centrale de la balle
   * @return Liste de points à vérifier
   */
  vector<Point> _collisionPoints(const Point& pos) const;


  /* ############## Détection de collision ############## */

  /**
   * @brief Vérifie si on peut ignorer la recherche de collision
   * @param pos Position actuelle de la balle
   * @param speed Vitesse de déplacement
   * @return true si aucune collision n'est possible
   */
  bool shouldSkipCollisionCheck(const Point& pos, float speed) const;

  /**
   * @brief Trouve la première collision sur le chemin
   * @param pos Position actuelle
   * @param direction Direction normalisée
   * @param speed Vitesse de la balle
   * @return Collision la plus proche
   * @complexity O(N) sur le nombre de briques
   */
  CollisionResult findClosestCollision(const Point& pos, const Point& direction, float speed);

  /**
   * @brief Teste l'intersection entre une brique et la trajectoire
   * @param brick Brique à tester
   * @param collisionPoints Points d'échantillonnage
   * @param direction Direction normalisée
   * @param speed Vitesse de la balle
   * @return Collision potentielle avec cette brique
   */
  CollisionResult checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const;

  /**
   * @brief Met à jour le score et l'état de la brique lors d'un impact
   * @param hitBrick Brique touchée
   */
  void handleBrickHit(Brick* hitBrick);

  /**
   * @brief Crée un bonus quand une brique est détruite
   * @param brick Brique détruite
   */
  void spawnBonus(const Brick& brick);

  /**
   * @brief Applique le rebond en fonction du côté touché
   * @param direction Direction de la balle à modifier
   * @param hitSide Côté de la brique (0 horizontal, 1 vertical, sinon coin)
   */
  void handleBallRebound(Point& direction, int hitSide);

 public:
  /**
   * @brief Constructeur
   * @param levelFile Fichier de niveau
   * @param score Score initial
   */
  GameModel(const string& levelFile, int score);

  /* ############## Méthodes de gestion du jeu ############## */

  /**
   * @brief Détecte et gère les collisions actuelles de la balle
   * @complexity O(N) sur le nombre de briques actives
   */
  void checkCollisions();

  /**
   * @brief Gestion du score
   */
  void saveHighScore();     // Sauvegarde le meilleur score
  void resetHighScore();    // Réinitialise le meilleur score
  void resetScore();        // Réinitialise le score actuel

  /**
   * @brief Gestion des bonus
   */
  void updateBonuses();         // Met à jour l'état des bonus
  void updateTimerBonuses();    // Met à jour les minuteurs des bonus
  BonusType getActiveBonus() const { return activeBonus; }
  void setActiveBonus(BonusType type) { activeBonus = type; }
  void clearActiveBonus();      // Supprime l'effet du bonus actif

   
  /* ############## Méthodes utilitaires ############## */

  /**
   * @brief Indique si la direction de la balle a changé
   * @param tempDirection Direction précédente, mise à jour en sortie
   * @return true si un changement est survenu
   */
  bool checkDirectionChanged(Point& tempDirection) const;

  /* ############## Accesseurs ############## */

  // Getters simples
  int getScore()           const;
  int getHighScore()       const;
  Point getBallDirection() const;

  // Accès aux entités
  Ball& getBall();
  const Ball& getBall() const;

  Spaceship& getSpaceship();
  const Spaceship& getSpaceship() const;

  const std::vector<Brick>&              getBricks()   const;
  const std::vector<shared_ptr<Bonus>>&  getBonuses() const;

  // État du jeu
  bool win()  const;  // Vérifie si le joueur a gagné
  bool lose() const;  // Vérifie si le joueur a perdu
};
