#pragma once
#include "json.hpp"
#include "model/utils.hpp"
#include "view/utils.hpp"
#include "Rectangle.hpp"


struct UIConfig {
  Point scorePos;
  Point highscorePos;
  Rectangle buttonPlay, buttonExit, buttonYes, buttonNo;
  std::unordered_map<int, ALLEGRO_COLOR> brickColors;

  UIConfig();
};
