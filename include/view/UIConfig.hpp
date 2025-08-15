#pragma once
#include "view/utils.hpp"


struct UIConfig {
  json settings;
  Point scorePos;
  Point highscorePos;
  Rectangle buttonPlay, buttonExit, buttonYes, buttonNo;

  UIConfig();
};
