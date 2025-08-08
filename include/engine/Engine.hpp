#pragma once
#include "view/utils.hpp"
#include "AllegroInputAdapter.hpp"
#include "GameController.hpp"
#include "GameModel.hpp"


class Engine {
 private:
  bool finish = false, showMenu = true;
  bool restartGame = false, done = false;

  std::vector<std::string>  gameLevels;
  shared_ptr<UIConfig>      uiConfig = nullptr;
  shared_ptr<AllegroConfig> allegroConfig = nullptr;

  // Handle Actions / Allegro event
  bool handleWin(shared_ptr<GameView> view, shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID);
  bool handleLose(shared_ptr<GameView> view, shared_ptr<GameModel> model, GameController& controller, ALLEGRO_SAMPLE_ID& soundID);
  bool processAction(const InputAction& action, GameController& controller, ALLEGRO_SAMPLE_ID& soundID);

  // Helpers
  void showFinalScreen(shared_ptr<GameView> view);
  bool initializeConfigs(shared_ptr<GameView> view);
  bool handleStartMenu(shared_ptr<GameView> view);

  bool runGameLevel(shared_ptr<GameModel> model, shared_ptr<GameView> view, GameController& controller, 
                    AllegroInputAdapter& inputAdapter, int index);

 public:
  Engine();
  ~Engine();
  
  void run();
};
