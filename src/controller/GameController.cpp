#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include "controller/GameController.hpp"


GameController::GameController(const std::shared_ptr<GameModel>& model, const std::shared_ptr<GameView>& view, const std::vector<std::string>& levelFiles)
    : model(model), view(view), levelFiles(levelFiles), gameScore(0), index(0), tempDirection(INITIAL_DIRECTION) {}

// Getters

[[gnu::pure]] int GameController::getIndex()                   const { return index;     }
[[gnu::pure]] int GameController::getGameScore()               const { return gameScore; }
[[gnu::pure]] std::shared_ptr<GameView> GameController::getView()   const { return view;      }
[[gnu::pure]] std::shared_ptr<GameModel> GameController::getModel() const { return model;     }

// Setters

void GameController::incrementIndex()  { index++;   }
void GameController::resetIndex()      { index = 0; }
void GameController::resetGameScore()  { gameScore = 0; }
void GameController::updateGameScore() { gameScore = model->getScore(); }

// Helpers

void GameController::resetTempDirection() {
  tempDirection = INITIAL_DIRECTION;
}

void GameController::resetGame() {
  Ball& ball = model->getBall();
  ball.reset();
  model->resetScore();
  model->resetHighScore();
  resetTempDirection();
}

void GameController::forceLevelChange(int idx) {
  index = idx;
  if (index >= 0 && index < static_cast<int>(levelFiles.size())) {
    updateGameScore();    // save score before loading
    resetTempDirection();
    *model = GameModel(levelFiles[static_cast<std::size_t>(index)], gameScore); // load chosen level
  }
}

// Manage Actions

void GameController::handleAction(const InputAction& action) {
  Spaceship& spaceship = model->getSpaceship();
  Ball& ball = model->getBall();

  switch (action.type) {
    case InputActionType::MoveLeft:
      spaceship.move(Direction::Left);
      break;

    case InputActionType::MoveRight:
      spaceship.move(Direction::Right);
      break;

    case InputActionType::Launch:
      ball.setMoving(true); 
      break;

    case InputActionType::Reset:
      resetGame();
      break;

    case InputActionType::LevelChange:
      forceLevelChange(action.x);
      
    case InputActionType::TimerTick:
    case InputActionType::Quit:
      break;
      
    case InputActionType::MouseMove: {
      float mx = static_cast<float>(action.x);
      float my = static_cast<float>(action.y);
      spaceship.move({mx, my}); // follow mouse position
      break;
    }
    default:break;
  }
}

// Advance the game simulation

void GameController::update() {
  std::shared_ptr<AllegroConfig> allegroConfig = view->getAllegroConfig();
  Spaceship& spaceship = model->getSpaceship();
  Ball& ball = model->getBall();

  model->checkCollisions();
  model->updateBonuses();
  model->updateSpaceship();

  if (ball.isFalling()) {  // ball missed the paddle
    resetTempDirection();  // prepare for next launch
    spaceship.damage();    // remove a life
    ball.reset();
    ball.setFalling(false);
    if (spaceship.getHealth()) { view->playSound(allegroConfig->fall_wav); }
  }
  // play a sound on rebound
  if (model->checkDirectionChanged(tempDirection)) { view->playSound(allegroConfig->bip_wav); }

  if (ball.isMoving()) ball.move(spaceship.getPosition(), spaceship.getWidth(), spaceship.getHeight());
  else                 ball.move(spaceship.getPosition(),spaceship.getHeight()); // keep ball above spaceship
}
