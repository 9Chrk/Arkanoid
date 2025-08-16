#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "engine/Engine.hpp"


Engine::Engine() : gameLevels(listLevels("./assets/data")) {
  if (gameLevels.empty()) throw std::runtime_error("No level files found.");
}

Engine::~Engine() {
  allegroConfig.reset();
}

// Handle Allegro Events

bool Engine::handleWin(std::shared_ptr<GameView> view, std::shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
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

bool Engine::handleLose(std::shared_ptr<GameView> view, std::shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID) {
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
  std::shared_ptr<GameView> view  = controller.getView();
  std::shared_ptr<GameModel> model = controller.getModel();

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

// Helpers for the RUN method

bool Engine::initializeConfigs(std::shared_ptr<GameView> view) {
  uiConfig = view->getUIConfig();
  allegroConfig = view->getAllegroConfig();
  if (!uiConfig || !allegroConfig) {
    std::cerr << "Fatal Error: UIConfig or AllegroConfig is null.\n";
    std::exit(EXIT_FAILURE);
  }
  return true;
}

bool Engine::handleStartMenu(std::shared_ptr<GameView> view) {
  view->menuButton(allegroConfig->start_png, done, uiConfig->buttonExit, uiConfig->buttonPlay);
  if (done) return false;
  showMenu = false;
  return true;
}

bool Engine::runGameLevel(std::shared_ptr<GameModel> model, std::shared_ptr<GameView> view, GameController& controller,
                          AllegroInputAdapter& inputAdapter, int index)
{
  *model = GameModel(gameLevels.at(static_cast<std::size_t>(index)), controller.getGameScore());
  controller.resetTempDirection();
  
  ALLEGRO_SAMPLE_ID soundGameID;
  view->playSound(allegroConfig->street_Fighter_wav, &soundGameID, true);
  done = restartGame = false;
  
  while (!done) {
    al_wait_for_event(allegroConfig->queue, &allegroConfig->event);
    if (auto act = inputAdapter.translate(allegroConfig->event)) {
      done = !processAction(*act, controller, soundGameID);
    }
  }
  
  model->saveHighScore();
  view->stopSound(&soundGameID);
  return restartGame;
}

void Engine::showFinalScreen(std::shared_ptr<GameView> view) {
  restartGame = false;
  finish = done = true;
  view->menu(allegroConfig->finish_png, "Press any key to exit...", allegroConfig->finish_wav, true);
}

// Game loop

void Engine::run() {
  AllegroInputAdapter inputAdapter;
  std::shared_ptr<GameModel> model = std::make_shared<GameModel>(gameLevels.at(0), 0);
  std::shared_ptr<GameView> view = std::make_shared<GameView>(model);
  GameController controller = GameController(model, view, gameLevels);
  
  if (!initializeConfigs(view)) return;
  
  while (true) {
    if (showMenu && !handleStartMenu(view)) break;
    
    int index = controller.getIndex();
    if (index >= static_cast<int>(gameLevels.size())) {
      showFinalScreen(view);
      break;
    }
    
    if (!runGameLevel(model, view, controller, inputAdapter, index)) break;
  }
}
