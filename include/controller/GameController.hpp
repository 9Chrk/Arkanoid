#pragma once
#include "model/GameModel.hpp"
#include "view/GameView.hpp"
#include "input/InputAction.hpp"


class GameController {
 private:
  std::shared_ptr<GameModel> model;
  std::shared_ptr<GameView>  view;

  const std::vector<std::string>& levelFiles;
  int gameScore, index;
  Point tempDirection;

  // Helpers
  void resetGame();
  void forceLevelChange(int idx);

 public:
  GameController(const std::shared_ptr<GameModel>& model,
                 const std::shared_ptr<GameView>& view,
                 const std::vector<std::string>& levelFiles);

  // Methods
  void handleAction(const InputAction& action);
  void update();
  
  // Helpers
  void resetTempDirection();

  // Getters
  std::shared_ptr<GameModel> getModel() const;
  std::shared_ptr<GameView> getView()   const;
  int getIndex()     const;
  int getGameScore() const;

  // Setters
  void resetGameScore();
  void updateGameScore();
  void resetIndex();
  void incrementIndex();
};
