#pragma once
#include "view/utils.hpp"


struct UIConfig {
  Point scorePos;
  Point highscorePos;
  Rectangle buttonPlay, buttonExit, buttonYes, buttonNo;
  std::unordered_map<int, ALLEGRO_COLOR> brickColors;

  UIConfig();
};
