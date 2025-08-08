#pragma once
#include "GameModel.hpp"
#include "GameView.hpp"


class GameController {
 private:
  std::shared_ptr<GameModel> model;
  std::shared_ptr<GameView>  view;

  const std::vector<std::string>& levelFiles;
  int gameScore, index;
  Point tempDirection;

 public:
  GameController(const std::shared_ptr<GameModel>& model,
                 const std::shared_ptr<GameView>& view,
                 const std::vector<std::string>& levelFiles);

  // Methods
  void handleEvent(const ALLEGRO_EVENT& event);
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
