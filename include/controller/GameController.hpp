#pragma once
#include "view/utils.hpp"
#include "GameModel.hpp"
#include "GameView.hpp"


class GameController {
 private:
  std::shared_ptr<GameModel> model;
  GameView view;

  const std::vector<std::string>& levelFiles;
  int totalScore;
  int levelIndex;

  Point tempDirection;

 public:
  GameController(std::shared_ptr<GameModel> model, int levelIndex, const std::vector<std::string>& levelFiles, int totalScore);

  void resetTempDirection();
  void handleEvent(const ALLEGRO_EVENT& event);
  void update();

  GameView& getView();
  std::shared_ptr<GameModel> getModel();
};
