#pragma once
#include "view/utils.hpp"
#include "GameController.hpp"
#include "GameModel.hpp"

class Engine {
 private:
  int index = 0, gameScore = 0;
  bool finish = false, showMenu = true;
  bool restartGame = false, done = false;
  std::vector<std::string> gameLevels;
  shared_ptr<UIConfig> uiConfig = nullptr;
  shared_ptr<AllegroConfig> allegroConfig = nullptr;

  // Print methods
  void printWin(int score, int highscore);
  void printLose(int score, int highscore);
  void printFinish(int score, int highscore);

  // Handle Allegro event
  bool processEvent(const ALLEGRO_EVENT& event, GameController& controller, ALLEGRO_SAMPLE_ID soundID);

 public:
  Engine();
  
  void run();
};
