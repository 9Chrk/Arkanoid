#pragma once
#include "json.hpp"
#include "model/utils.hpp"
#include "view/utils.hpp"
#include "Rectangle.hpp"


struct UIConfig {
  Point scorePos;
  Point highscorePos;
  Rectangle buttonPlay, buttonExit, buttonYes, buttonNo;

  UIConfig();
};
