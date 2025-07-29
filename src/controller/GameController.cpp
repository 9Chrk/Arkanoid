using namespace std;
#include "GameController.hpp"


GameController::GameController(shared_ptr<GameModel> model, int levelIndex, const vector<string>& levelFiles, int totalScore)
    : model(model), view(GameView(model)), levelFiles(levelFiles), totalScore(totalScore), levelIndex(levelIndex), tempDirection({0, -1}) {}

// Getters
 
GameView& GameController::getView() {
  return view;
}

shared_ptr<GameModel> GameController::getModel() {
  return model;
}

// Helper methods

void GameController::resetTempDirection() {
  tempDirection = {0, -1};
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
        spaceship.move(0);
        break;

      case ALLEGRO_KEY_RIGHT:
      case ALLEGRO_KEY_D:
      case ALLEGRO_KEY_P:
        spaceship.move(1);
        break;

      case ALLEGRO_KEY_SPACE:
        ball.setMouvement(true);
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
        int level = event.keyboard.keycode - ALLEGRO_KEY_0;
        if (level >= 0 && level < static_cast<int>(levelFiles.size())) {
          *model = GameModel(levelFiles[static_cast<size_t>(level)], totalScore);
          resetTempDirection();
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
  Spaceship& spaceship = model->getSpaceship();
  Ball& ball = model->getBall();

  model->checkCollisions();

  if (ball.isFalling()) {
    spaceship.damage();
    ball.reset();
    ball.setFalling(false);
  }

  if (ball.inMouvement()) {
    ball.move(spaceship.getPosition(),
              spaceship.getWidth(),
              spaceship.getHeight());
  } else {
    ball.move(spaceship.getPosition(),
              spaceship.getHeight());
  }

  view.render();

  if (model->checkDirectionChanged(tempDirection)) {
    if (auto allegroConfig = view.getAllegroConfig()) {
      view.playSound(allegroConfig->bip_wav.get());
    }
  }
}
