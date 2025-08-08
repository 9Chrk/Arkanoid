using namespace std;
#include "Engine.hpp"


Engine::Engine() : gameLevels(listLevels("./assets/data")) {
  if (gameLevels.empty()) throw runtime_error("No level files found.");
}

Engine::~Engine() {
  allegroConfig.reset();
}

// Handle Allegro Events

bool Engine::processEvent(const ALLEGRO_EVENT& event, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
  shared_ptr<GameView> view  = controller.getView();   // current view
  shared_ptr<GameModel> model = controller.getModel(); // game state

  // Quit if the user closes the window or hits Escape
  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
     (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
    return false;
  }

  // Forward keyboard and mouse events to the controller
  if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES) {
    controller.handleEvent(event);
  }

  // Timer event: advance simulation and redraw frame
  if (event.type == ALLEGRO_EVENT_TIMER) {
    controller.update();
    view->render();
  }

  // Player completed current level
  if (model->win()) {
    view->stopSound(&soundID);
    view->menu(allegroConfig->win_png, "Press any key to continue...", allegroConfig->win_wav);
    controller.incrementIndex();
    controller.updateGameScore();
    restartGame = true; // next level should start
    return false;
  }

  // Player lost all lives
  if (model->lose()) {
    view->stopSound(&soundID);
    view->playSound(allegroConfig->lose_wav);
    view->menuButton(allegroConfig->lose_png, restartGame, uiConfig->buttonYes, uiConfig->buttonNo);
    controller.resetGameScore();
    controller.resetIndex();
    return false; // back to menu
  }

  return true; // keep processing events
}

// Game loop

void Engine::run() {
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

      // Process events until level ends
      while (!done) {
        ALLEGRO_EVENT event = allegroConfig->event;
        al_wait_for_event(allegroConfig->queue, &event); // wait for next event
        done = !processEvent(event, controller, soundGameID);
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
