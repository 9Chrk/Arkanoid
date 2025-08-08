#pragma once
#include "view/utils.hpp"
#include "GameController.hpp"
#include "GameModel.hpp"


class Engine {
 private:
  bool finish = false, showMenu = true;
  bool restartGame = false, done = false;

  std::vector<std::string>  gameLevels;
  shared_ptr<UIConfig>      uiConfig = nullptr;
  shared_ptr<AllegroConfig> allegroConfig = nullptr;

  // Handle Allegro event
  bool processEvent(const ALLEGRO_EVENT& event, GameController& controller, ALLEGRO_SAMPLE_ID& soundID);

 public:
  Engine();
  ~Engine();
  
  void run();
};
