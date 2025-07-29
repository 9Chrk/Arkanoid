#include "Engine.hpp"


Engine::Engine()
  : gameLevels(executeCommand("./research_jsonFile.sh ./assets/data level_")) {
  if (gameLevels.empty()) {
    std::cerr << "Error: no level found by research_jsonFile.sh\n";
    std::exit(EXIT_FAILURE);
  }
}

// Print methods

void Engine::printWin(int score, int highscore) {
  std::cout << "\nCongratulations! You've broken every brick! 🎇" << std::endl;
  std::cout << "Score achieved : " << score << std::endl;
  std::cout << "Highscore      : " << highscore << "\n" << std::endl;
}

void Engine::printLose(int score, int highscore) {
  std::cout << "\nGame Over... The bricks have won this time. 🧱" << std::endl;
  std::cout << "Score achieved : " << score << std::endl;
  std::cout << "Highscore      : " << highscore << "\n" << std::endl;
}

void Engine::printFinish(int score, int highscore) {
  std::cout << "\nYou have finished ARKANOID !!!" << std::endl;
  std::cout << "Score achieved : " << score << std::endl;
  std::cout << "Highscore      : " << highscore << "\n" << std::endl;
}

// Handle Allegro Events

bool Engine::processEvent(const ALLEGRO_EVENT& event, GameController& controller, ALLEGRO_SAMPLE_ID soundID) {
  GameView& view = controller.getView();
  std::shared_ptr<GameModel> model = controller.getModel();

  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || 
     (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
    return false;
  }

  if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES) {
    controller.handleEvent(event);
  }

  if (event.type == ALLEGRO_EVENT_TIMER) {
    controller.update();
  }

  if (model->win()) {
    view.stopSound(&soundID);
    printWin(model->getScore(), model->getHighScore());
    view.menu(allegroConfig->win_png, "Press any key to continue...");
    index++;
    restartGame = true;
    gameScore = model->getScore();
    return false;
  }

  if (model->lose()) {
    view.stopSound(&soundID);
    view.playSound(allegroConfig->lose_wav);
    printLose(model->getScore(), model->getHighScore());
    view.menuButton(allegroConfig->lose_png, restartGame, uiConfig->buttonYes, uiConfig->buttonNo);
    gameScore = 0;
    index = 0;
    return false;
  }

  return true;
}

// Game loop

void Engine::run() {
  auto model = std::make_shared<GameModel>(gameLevels.at(static_cast<size_t>(index)), gameScore);
  GameController controller = GameController(model, index, gameLevels, gameScore);
  GameView& view = controller.getView();

  uiConfig = view.getUIConfig();
  allegroConfig = view.getAllegroConfig();

  if (!uiConfig || !allegroConfig) {
    std::cerr << "Fatal Error: UIConfig or AllegroConfig is null.\n";
    std::exit(EXIT_FAILURE);
  }

  while (true) {
    if (showMenu) {
      view.menuButton(allegroConfig->start_png, done, uiConfig->buttonExit, uiConfig->buttonPlay);
      if (done) break;
      showMenu = false;
    }

    if (index < static_cast<int>(gameLevels.size())) {
      *model = GameModel(gameLevels.at(static_cast<size_t>(index)), gameScore);
      controller.resetTempDirection();
    } else {
      done = true;
      restartGame = false;
      finish = true;
    }

    ALLEGRO_SAMPLE_ID soundGameID;
    view.playSound(allegroConfig->Street_Fighter_wav, &soundGameID);

    while (!done) {
      ALLEGRO_EVENT event = allegroConfig->event;
      al_wait_for_event(allegroConfig->queue, &event);
      if (!processEvent(event, controller, soundGameID)) {
        done = true;
      }
    }

    model->saveHighScore();
    view.stopSound(&soundGameID);
    if (!restartGame) break;
  }

  if (finish) {
    printFinish(model->getScore(), model->getHighScore());
    view.menu(allegroConfig->finish_png, "Press any key to exit...");
  }
}
