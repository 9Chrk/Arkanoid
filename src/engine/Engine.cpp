using namespace std;
#include "Engine.hpp"


Engine::Engine() : gameLevels(listLevels("./assets/data")) {
  if (gameLevels.empty()) {
    cerr << "Error: no level found by research_jsonFile.sh\n";
    exit(EXIT_FAILURE);
  }
}

Engine::~Engine() {
  allegroConfig.reset();
}

// Handle Allegro Events

bool Engine::processEvent(const ALLEGRO_EVENT& event, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
  shared_ptr<GameView> view = controller.getView();
  shared_ptr<GameModel> model = controller.getModel();

  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || 
     (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
    return false;
  }

  if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_MOUSE_AXES) {
    controller.handleEvent(event);
  }

  if (event.type == ALLEGRO_EVENT_TIMER) {
    controller.update();
    view->render();
  }

  if (model->win()) {
    view->stopSound(&soundID);
    view->menu(allegroConfig->win_png, "Press any key to continue...", allegroConfig->win_wav);
    controller.incrementIndex();
    controller.updateGameScore();
    restartGame = true;
    return false;
  }

  if (model->lose()) {
    view->stopSound(&soundID);
    view->playSound(allegroConfig->lose_wav);
    view->menuButton(allegroConfig->lose_png, restartGame, uiConfig->buttonYes, uiConfig->buttonNo);
    controller.resetGameScore();
    controller.resetIndex();
    return false;
  }

  return true;
}

// Game loop

void Engine::run() {
  auto model = make_shared<GameModel>(gameLevels.at(0), 0);
  auto view = make_shared<GameView>(model);
  GameController controller = GameController(model, view, gameLevels);

  uiConfig = view->getUIConfig();
  allegroConfig = view->getAllegroConfig();
  
  if (!uiConfig || !allegroConfig) {
    cerr << "Fatal Error: UIConfig or AllegroConfig is null.\n";
    exit(EXIT_FAILURE);
  }
  
  while (true) {
    if (showMenu) {
      view->menuButton(allegroConfig->start_png, done, uiConfig->buttonExit, uiConfig->buttonPlay);
      if (done) break;
      showMenu = false;
    }
    
    int index = controller.getIndex();
    if (index < static_cast<int>(gameLevels.size())) {
      *model = GameModel(gameLevels.at(static_cast<size_t>(index)), controller.getGameScore());
      controller.resetTempDirection();
      
      ALLEGRO_SAMPLE_ID soundGameID;
      view->playSound(allegroConfig->street_Fighter_wav, &soundGameID, true);
      done = restartGame = false;
      
      while (!done) {
        ALLEGRO_EVENT event = allegroConfig->event;
        al_wait_for_event(allegroConfig->queue, &event);
        done = !processEvent(event, controller, soundGameID);
      }
      
      model->saveHighScore();
      view->stopSound(&soundGameID);
      if (!restartGame) break;
      
    } else {
      restartGame = false;
      finish = done = true;
      break;
    }
  }
  
  if (finish) {
    view->menu(allegroConfig->finish_png, "Press any key to exit...", allegroConfig->finish_wav, true);
  }
}
