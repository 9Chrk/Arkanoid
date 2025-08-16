#pragma once
#include "utils.hpp"


struct UIConfig {
  json settings;
  Point scorePos;
  Point highscorePos;
  Rectangle buttonPlay, buttonExit, buttonYes, buttonNo;

  UIConfig();
};
