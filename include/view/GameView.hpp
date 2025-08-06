#pragma once
#include "view/utils.hpp"
#include "Bonus.hpp"
#include "GameModel.hpp"
#include "AllegroConfig.hpp"
#include "UIConfig.hpp"


class GameView {
 private:
  std::shared_ptr<GameModel> model;
  std::shared_ptr<UIConfig> uiConfig;
  std::shared_ptr<AllegroConfig> allegroConfig;

  // Draw methods
  void drawCircle(const Point& position, float radius, ALLEGRO_COLOR color) const;
  void drawRectangle(const Point& upLeft, const Point& downRight, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR frameColor) const;
  void drawBall() const;
  void drawBrick(const Brick& brick) const;
  void drawBricks() const;
  void drawBonusAbbreviation(const Brick& brick) const;
  void drawBonus(const Bonus& bonus) const;
  void drawBonuses() const;

  // Display methods
  void displayImage(ALLEGRO_BITMAP* bmp) const;
  void displayScore() const;
  void displayHUD() const;
  void displaySpaceship() const;

  // Helper methods
  ALLEGRO_BITMAP* chooseHeartFile() const;

  void handleButtonHover(ALLEGRO_BITMAP* image_png, bool hoverPlay, bool hoverExit, 
                       const Rectangle& buttonON, const Rectangle& buttonOFF);
  bool processMenuMouseEvent(const ALLEGRO_EVENT& event, ALLEGRO_BITMAP* image_png,
                           bool& hoverPlay, bool& hoverExit, bool& done,
                           const Rectangle& buttonON, const Rectangle& buttonOFF);

 public:
  GameView(shared_ptr<GameModel> model);

  // Sounds methods
  void playSound(ALLEGRO_SAMPLE* sound, ALLEGRO_SAMPLE_ID* id=nullptr, bool loop=false) const;
  void stopSound(ALLEGRO_SAMPLE_ID* id) const;

  // Render methods
  void render() const;

  // Menu methods
  void menu(ALLEGRO_BITMAP* image_png, const std::string& text, ALLEGRO_SAMPLE* sound, bool soundLoop=false, bool showScore=false);
  void menuButton(ALLEGRO_BITMAP* image_png, bool& done, const Rectangle& buttonON, const Rectangle& buttonOFF);
  
  // Getters
  std::shared_ptr<UIConfig> getUIConfig() const;
  std::shared_ptr<AllegroConfig> getAllegroConfig() const;
};
