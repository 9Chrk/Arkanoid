using namespace std;
#include "GameController.hpp"

GameController::GameController(const shared_ptr<GameModel>& model,
                               const shared_ptr<GameView>& view,
                               const vector<string>& levelFiles)
    : model(model), view(view), levelFiles(levelFiles), gameScore(0), index(0), tempDirection(INITIAL_DIRECTION) {}

// Getters

[[gnu::pure]] int GameController::getIndex()                   const { return index;     }
[[gnu::pure]] int GameController::getGameScore()               const { return gameScore; }
[[gnu::pure]] shared_ptr<GameView> GameController::getView()   const { return view;      }
[[gnu::pure]] shared_ptr<GameModel> GameController::getModel() const { return model;     }

// Setters

void GameController::incrementIndex()  { index++;   }
void GameController::resetIndex()      { index = 0; }
void GameController::resetGameScore()  { gameScore = 0; }
void GameController::updateGameScore() { gameScore = model->getScore(); }

// Helper methods

void GameController::resetTempDirection() {
  tempDirection = INITIAL_DIRECTION;
}

// Manage Allegro events

void GameController::handleEvent(const ALLEGRO_EVENT& event) {
  Spaceship& spaceship = model->getSpaceship();
  Ball& ball = model->getBall();

  if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
    switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_LEFT:
      case ALLEGRO_KEY_A:
      case ALLEGRO_KEY_Q:
        spaceship.move(Direction::Left);
        break;

      case ALLEGRO_KEY_RIGHT:
      case ALLEGRO_KEY_D:
      case ALLEGRO_KEY_P:
        spaceship.move(Direction::Right);
        break;

      case ALLEGRO_KEY_SPACE:
        ball.setMoving(true);
        break;

      case ALLEGRO_KEY_R:
        ball.reset();
        model->resetScore();
        model->resetHighScore();
        resetTempDirection();
        break;

      case ALLEGRO_KEY_0: case ALLEGRO_KEY_1: case ALLEGRO_KEY_2:
      case ALLEGRO_KEY_3: case ALLEGRO_KEY_4: case ALLEGRO_KEY_5:
      case ALLEGRO_KEY_6: case ALLEGRO_KEY_7: case ALLEGRO_KEY_8: {
        index = event.keyboard.keycode - ALLEGRO_KEY_0;
        if (index >= 0 && index < static_cast<int>(levelFiles.size())) {
          updateGameScore();
          resetTempDirection();
          *model = GameModel(levelFiles[static_cast<size_t>(index)], gameScore);
        }
        break;
      }
      default:
        break;
    }
  }
  else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
    float mx = static_cast<float>(event.mouse.x);
    float my = static_cast<float>(event.mouse.y);
    spaceship.move({mx, my});
  }
}

// Advance the game simulation

void GameController::update() {
  shared_ptr<AllegroConfig> allegroConfig = view->getAllegroConfig();
  Spaceship& spaceship = model->getSpaceship();
  Ball& ball = model->getBall();

  model->checkCollisions();

  if (ball.isFalling()) {
    spaceship.damage();
    ball.reset();
    ball.setFalling(false);
    if (spaceship.getHealth()) { view->playSound(allegroConfig->fall_wav); }
    resetTempDirection();
  }

  if (ball.inMouvement()) {
    ball.move(spaceship.getPosition(),
              spaceship.getWidth(),
              spaceship.getHeight());
  }
  else {
    ball.move(spaceship.getPosition(),
              spaceship.getHeight()); // keep ball above spaceship
  }

  if (model->checkDirectionChanged(tempDirection)) {
    view->playSound(allegroConfig->bip_wav);
  }
}
