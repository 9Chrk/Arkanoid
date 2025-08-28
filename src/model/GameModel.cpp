using namespace std;
#include "model/GameModel.hpp"
#include "model/Bonus/Laser.hpp"
#include "model/Bonus/Expand.hpp"
#include "model/Bonus/Catch.hpp"
#include "model/Bonus/SlowDown.hpp"
#include "model/Bonus/Split.hpp"
#include "model/Bonus/ExtraLife.hpp"


/* ############## Constructeur et initialisation ############## */

/**
 * @brief Constructeur principal du modèle de jeu
 * Initialise les composants du jeu depuis les fichiers de configuration
 */
GameModel::GameModel(const string& levelFile, int score)
         : settings(loadSettings()), score(score), highscore(settings["highscore"].get<int>()),
           levelFile(levelFile), ball(), spaceship(), bricks(), bonuses() {
  initializeBall();
  initializeSpaceship();
  initializeBricks();
}

/**
 * @brief Initialise la balle avec les paramètres du fichier de configuration
 */
void GameModel::initializeBall()
{
  json _ball                    = settings["ball"];
  float radius                  = _ball["radius"].get<float>();
  float speed                   = _ball["speed"].get<float>();
  ball                          = Ball(radius, speed);
}

/**
 * @brief Initialise le vaisseau avec les paramètres du fichier de configuration
 */
void GameModel::initializeSpaceship()
{
  json _spaceship               = settings["spaceship"];
  float position_x              = _spaceship["position.x"].get<float>();
  float position_y              = _spaceship["position.y"].get<float>();
  float width                   = _spaceship["width"].get<float>();
  float height                  = _spaceship["height"].get<float>();
  float speed                   = _spaceship["speed"].get<float>();
  int health                    = _spaceship["health"].get<int>();
  spaceship                     = Spaceship({position_x, position_y}, width, height, speed, health);
}

/**
 * @brief Initialise les briques à partir du fichier de niveau
 * Crée la disposition des briques avec leurs attributs (points, bonus)
 */
void GameModel::initializeBricks()
{
  json level                    = openJsonFile("./assets/data/" + levelFile);
  vector<string> bricks_data    = level["bricks"].get<vector<string>>();
  const int dim_x               = level["dim_x"].get<int>();
  const int width               = settings["brick"]["width"].get<int>();
  const int height              = settings["brick"]["height"].get<int>();
  const int margin              = settings["brick"]["margin"].get<int>();
  const int _offset_start_x     = settings["display_offset"]["offset_start_x"].get<int>();
  const int _offset_start_y     = settings["display_offset"]["offset_start_y"].get<int>();

  // Calcul des positions
  const float offset_x          = static_cast<float>(width + margin);
  const float offset_y          = static_cast<float>(height + margin);
  const float total_width       = static_cast<float>(dim_x * width + (dim_x - 1) * margin);
  const float offset_start_x    = (_offset_start_x > 0) ? static_cast<float>(_offset_start_x) : (GAME_WIDTH - total_width)/2.0f; // center the grid if no offset
  const float offset_start_y    = static_cast<float>(_offset_start_y);

  // Création des briques
  for (size_t index = 0; index < bricks_data.size(); ++index) {
    size_t i = index / static_cast<size_t>(dim_x);
    size_t j = index % static_cast<size_t>(dim_x);
    
    pair<string, string> info = splitAtSeparator(bricks_data.at(index));
    int score = stoi(info.first);
    BonusType bonusType = (info.second.empty() ? BonusType::NONE : Bonus::fromAbbreviation(info.second));

    if (score != EMPTY_BRICK) {
      bricks.emplace_back(
        Point({offset_start_x + offset_x * static_cast<float>(j) + offset_x/2,
               offset_start_y + offset_y * static_cast<float>(i) + offset_y/2}),
        width, height, score, bonusType
      );
    }
  }
}

/**
 * @brief Divise une chaîne au niveau du séparateur '|'
 * Utilisé pour extraire le score et le type de bonus des briques
 */
pair<string, string> GameModel::splitAtSeparator(const string& str) {
  const size_t pos = str.find('|');
  if (pos == string::npos) return {str, "" };
  return {str.substr(0, pos), str.substr(pos + 1)};
}

/* ############## Système de collision ############## */

/**
 * @brief Détecte et gère les collisions de la balle avec les briques
 * Cette fonction centrale trouve la première brique touchée et applique les effets
 */
void GameModel::checkCollisions() {
  Point pos = ball.getPosition();
  float speed = ball.getSpeed();
  Point direction = Vector::normalize(ball.getDirection());
  
  // Optimisation: évite la recherche si pas de collision possible
  if (shouldSkipCollisionCheck(pos, speed)) return;

  // Trouve la première collision sur le trajet de la balle
  CollisionResult collision = findClosestCollision(pos, direction, speed);

  // Aucune collision détectée
  if (!collision.hitBrick) return;

  // Gestion de l'impact
  handleBrickHit(collision.hitBrick);       // Score et destruction
  handleBallRebound(direction, collision.hitSide);  // Rebond
}

/**
 * @brief Vérifie si on peut éviter la recherche de collision
 * Optimisation qui évite les calculs inutiles quand la balle est trop basse
 */
bool GameModel::shouldSkipCollisionCheck(const Point& pos, float speed) const {
  const Brick &last = bricks.back();
  return pos.y >= last.getPosition().y + (last.getHeight()/2) + (speed * speed);
}

/**
 * @brief Trouve la collision la plus proche sur la trajectoire de la balle
 * Parcourt toutes les briques actives pour trouver le premier impact
 */
GameModel::CollisionResult GameModel::findClosestCollision(const Point& pos, const Point& direction, float speed) {
  const vector<Point> collisionPoints = _collisionPoints(pos);
  CollisionResult result;
  
  // Test de collision pour chaque brique
  for (Brick &brick : bricks) {
    if (brick.isDestroyed()) continue;
    
    CollisionResult brickCollision = checkBrickCollision(brick, collisionPoints, direction, speed);
    if (brickCollision.hitBrick && brickCollision.distance < result.distance) {
      result = brickCollision;
    }
  }
  return result;
}

/**
 * @brief Teste la collision entre une brique et la trajectoire de la balle
 * Utilise plusieurs points de test autour de la balle pour une détection précise
 */
GameModel::CollisionResult GameModel::checkBrickCollision(Brick& brick, const vector<Point>& collisionPoints, const Point& direction, float speed) const {
  CollisionResult result;
  
  // Teste chaque point de collision
  for (const Point &p : collisionPoints) {
    Point nextP = {p.x + direction.x * speed, p.y + direction.y * speed};
    pair<Point, int> intersectionResult = brick.getVector().intersection({p, nextP});
    int side = intersectionResult.second;
    
    // Pas d'intersection
    if (side == -1) continue;
    
    // Trouve la collision la plus proche
    float distance = Vector::distance(p, intersectionResult.first);
    if (distance < result.distance) {
      result.hitSide = side;
      result.hitBrick = &brick;
      result.distance = distance;
    }
  }
  return result;
}

/**
 * @brief Gère l'impact avec une brique
 * Met à jour le score et détruit la brique si nécessaire
 */
void GameModel::handleBrickHit(Brick* hitBrick) {
  // Traitement selon le type de brique
  if (hitBrick->getScore() != SECOND_LIFE || !hitBrick->getSecondLife()) {
    if (hitBrick->getScore() != GOLD_BRICK) {
      hitBrick->destroy();
      score += hitBrick->getScore();
      spawnBonus(*hitBrick);
    }
  } else {
    hitBrick->setSecondLife(false);
  }
}

/**
 * @brief Modifie la direction de la balle selon le côté touché
 * Différents rebonds selon que la balle touche un côté horizontal, vertical ou un coin
 */
void GameModel::handleBallRebound(Point& direction, int hitSide) {
  if (hitSide == 1) {
    direction.x *= -1;      // Rebond sur côté vertical
  } else if (hitSide == 0) {
    direction.y *= -1;      // Rebond sur côté horizontal
  } else {
    direction.x *= -1;      // Rebond sur coin (inverse les deux directions)
    direction.y *= -1;
  }
  ball.setDirection(direction);
}

/**
 * @brief Crée un bonus quand une brique est détruite
 * Le type de bonus dépend de l'attribut de la brique
 */
void GameModel::spawnBonus(const Brick& brick) {
  BonusType type = brick.getBonus();
  if (type == BonusType::NONE) return;

  // Paramètres du bonus
  json bonusSettings = settings["bonus"];
  float width  = bonusSettings["width"].get<float>();
  float height = bonusSettings["height"].get<float>();
  float speed  = bonusSettings["speed"].get<float>();
  Point position = brick.getPosition();

  // Création du bonus selon son type
  shared_ptr<Bonus> bonus;
  switch (type) {
    case BonusType::LASER:      bonus = make_shared<Laser>(position, width, height, speed); break;
    case BonusType::EXPAND:     bonus = make_shared<Expand>(position, width, height, speed); break;
    case BonusType::CATCH:      bonus = make_shared<Catch>(position, width, height, speed); break;
    case BonusType::SLOW_DOWN:  bonus = make_shared<SlowDown>(position, width, height, speed); break;
    case BonusType::SPLIT:      bonus = make_shared<Split>(position, width, height, speed); break;
    case BonusType::EXTRA_LIFE: bonus = make_shared<ExtraLife>(position, width, height, speed); break;
    default: return;
  }

  // Activation et ajout du bonus
  bonus->setActive(true);
  bonuses.push_back(bonus);
}

/* ############## Gestion des bonus ############## */

/**
 * @brief Met à jour tous les bonus actifs et vérifie les collisions avec le vaisseau
 */
void GameModel::updateBonuses() {
    for (auto it = bonuses.begin(); it != bonuses.end(); ) {
        shared_ptr<Bonus> bonus = *it;
        if (bonus->isActive()) {
            bonus->update();
            if (bonus->intersects(spaceship)) {
                // Désactiver le bonus précédent avant d'appliquer le nouveau
                clearActiveBonus();

                // Appliquer le nouveau bonus
                bonus->setCollected(true);
                bonus->applyEffect(*this);

                // Définir ce bonus comme actif
                setActiveBonus(bonus->getType());

                it = bonuses.erase(it);
                continue;
            }
        }
        ++it;
    }
}

/**
 * @brief Met à jour les minuteurs des bonus actifs
 */
void GameModel::updateTimerBonuses() {
  spaceship.updateExpandTimer();
  ball.updateCatchReleaseTimer();
  ball.updateSpeed();
}

/**
 * @brief Désactive le bonus actuel et restaure l'état normal
 */
void GameModel::clearActiveBonus() {
    switch (activeBonus) {
        case BonusType::EXPAND:
            spaceship.setWidth(spaceship.getoriginalWidth());
            spaceship.setExpandTimer(0);
            break;
        case BonusType::CATCH:
            ball.setCatchActive(false);
            ball.setCatchReleaseTimer(0);
            break;
        case BonusType::SLOW_DOWN:
            ball.setSpeed(ball.getOriginalSpeed());
            break;
            // Ajouter les autres types de bonus...
        default:
            break;
    }
    activeBonus = BonusType::NONE;
}

/* ############## Méthodes utilitaires ############## */

/**
 * @brief Génère des points autour de la balle pour la détection de collision
 * Permet une détection plus précise en testant plusieurs points (centre et bords)
 */
[[gnu::pure]] vector<Point> GameModel::_collisionPoints(const Point& pos) const {
  vector<Point> collisionPoints = {
    {pos.x, pos.y},
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}, // Top-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Left
    {static_cast<float>(pos.x + ball.getRadius() / sqrt(2)), static_cast<float>(pos.y + ball.getRadius() / sqrt(2))}, // Bottom-Right
    {static_cast<float>(pos.x - ball.getRadius() / sqrt(2)), static_cast<float>(pos.y - ball.getRadius() / sqrt(2))}  // Top-Left
  };
  return collisionPoints;
}

/**
 * @brief Vérifie si la direction de la balle a changé
 * Utilisé pour optimiser les rendus graphiques
 */
bool GameModel::checkDirectionChanged(Point& tempDirection) const {
  const Point& currentDirection = ball.getDirection();
  constexpr float eps = numeric_limits<float>::epsilon(); // floating-point tolerance
  if (fabs(currentDirection.x - tempDirection.x) > eps ||
      fabs(currentDirection.y - tempDirection.y) > eps) {
    tempDirection = currentDirection;
    return true;
  }
  return false;
}

/* ############## Gestion du score ############## */

/**
 * @brief Sauvegarde le meilleur score si le score actuel est plus élevé
 */
void GameModel::saveHighScore() {
  if (score >= highscore) {
    highscore = score;
    writeJsonFile("./assets/data/settings.json", "highscore", highscore);
  }
}

/**
 * @brief Réinitialise le meilleur score
 */
void GameModel::resetHighScore() {
  highscore = 0;
  writeJsonFile("./assets/data/settings.json", "highscore", highscore);
}

/**
 * @brief Réinitialise le score actuel
 */
void GameModel::resetScore() {
  score = 0; 
}

/* ############## Accesseurs ############## */

// Getters simples
[[gnu::pure]] int GameModel::getScore()           const { return score; }
[[gnu::pure]] int GameModel::getHighScore()       const { return highscore; }
[[gnu::pure]] Point GameModel::getBallDirection() const { return ball.getDirection(); }

// Accès aux entités
[[gnu::pure]] const Ball& GameModel::getBall()           const { return ball; }
[[gnu::pure]] const Spaceship& GameModel::getSpaceship() const { return spaceship; }

[[gnu::pure]] Ball& GameModel::getBall() { return ball; }
[[gnu::pure]] Spaceship& GameModel::getSpaceship() { return spaceship; }

[[gnu::pure]] const vector<Brick>& GameModel::getBricks()  const { return bricks;  }
[[gnu::pure]] const vector<shared_ptr<Bonus>>& GameModel::getBonuses() const { return bonuses; }

// État du jeu
[[gnu::pure]] bool GameModel::lose() const { return spaceship.isDeath(); }

[[gnu::pure]] bool GameModel::win()  const {
  for (auto& brick : bricks) if (!brick.isDestroyed() && brick.getScore() != 0) return false;
  return true;
}
