using namespace std;
#include "Engine.hpp"


Engine::Engine() : gameLevels(listLevels("./assets/data")) {
  if (gameLevels.empty()) throw runtime_error("No level files found.");
}

Engine::~Engine() {
  allegroConfig.reset();
}

// Handle Allegro Events

bool Engine::handleWin(shared_ptr<GameView> view, shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
  if (model->win()) {
    view->stopSound(&soundID);
    view->menu(allegroConfig->win_png, "Press any key to continue...", allegroConfig->win_wav);
    controller.incrementIndex();
    controller.updateGameScore();
    restartGame = true;
    return true;
  }
  return false;
}

bool Engine::handleLose(shared_ptr<GameView> view, shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
  if (model->lose()) {
    view->stopSound(&soundID);
    view->playSound(allegroConfig->lose_wav);
    view->menuButton(allegroConfig->lose_png, restartGame, uiConfig->buttonYes, uiConfig->buttonNo);
    controller.resetGameScore();
    controller.resetIndex();
    return true;
  }
  return false;
}

bool Engine::processAction(const InputAction& action, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
  shared_ptr<GameView> view  = controller.getView();
  shared_ptr<GameModel> model = controller.getModel();

  switch (action.type) {
    case InputActionType::Quit:
      return false;

    case InputActionType::TimerTick:
      controller.update();
      view->render();
      break;

    case InputActionType::MoveLeft:
    case InputActionType::MoveRight:
    case InputActionType::Launch:
    case InputActionType::Reset:
    case InputActionType::MouseMove:
    case InputActionType::LevelChange:
      controller.handleAction(action);
      break;

    default: break;
  }
  if (handleWin(view, model, controller, soundID) || handleLose(view, model, controller, soundID)) { 
    return false; 
  }
  return true;
}

// Game loop

void Engine::run() {
  ALLEGRO_EVENT event;
  AllegroInputAdapter inputAdapter;

  auto model = make_shared<GameModel>(gameLevels.at(0), 0); // start with first level
  auto view  = make_shared<GameView>(model);
  GameController controller = GameController(model, view, gameLevels);

  uiConfig      = view->getUIConfig();      // UI layout and graphics
  allegroConfig = view->getAllegroConfig(); // Allegro resources

  if (!uiConfig || !allegroConfig) {
    cerr << "Fatal Error: UIConfig or AllegroConfig is null.\n";
    exit(EXIT_FAILURE);
  }

  while (true) {
    // Show start menu once at launch
    if (showMenu) {
      view->menuButton(allegroConfig->start_png, done, uiConfig->buttonExit, uiConfig->buttonPlay);
      if (done) break;          // user chose to quit
      showMenu = false;         // otherwise start game
    }

    int index = controller.getIndex();
    if (index < static_cast<int>(gameLevels.size())) {
      *model = GameModel(gameLevels.at(static_cast<size_t>(index)), controller.getGameScore()); // load level
      controller.resetTempDirection();

      ALLEGRO_SAMPLE_ID soundGameID;
      view->playSound(allegroConfig->street_Fighter_wav, &soundGameID, true); // background music
      done = restartGame = false;

      event = allegroConfig->event;
      // Process events until level ends
      while (!done) {
        al_wait_for_event(allegroConfig->queue, &event); // wait for next event
        if (auto act = inputAdapter.translate(event)) { done = !processAction(*act, controller, soundGameID); }
      }

      model->saveHighScore();
      view->stopSound(&soundGameID);
      if (!restartGame) break; // exit loop if player doesn't restart

    } else {
      // No more levels: show final screen
      restartGame = false;
      finish = done = true;
      break;
    }
  }

  if (finish) {
    view->menu(allegroConfig->finish_png, "Press any key to exit...", allegroConfig->finish_wav, true); // goodbye screen
  }
}
