#include "UIConfig.hpp"
#include "core/utils.hpp"
#include "view/utils.hpp"


UIConfig::UIConfig()
  : settings(loadSettings()),
    scorePos(),
    highscorePos(),
    buttonPlay(),
    buttonExit(),
    buttonYes(),
    buttonNo()
{
  json displayButton = settings["display_button"];
  json display_score = settings["display_score"];

  scorePos     = {display_score["score.x"].get<float>(),     display_score["score.y"].get<float>()};
  highscorePos = {display_score["highscore.x"].get<float>(), display_score["highscore.y"].get<float>()};

  buttonPlay   = Rectangle({displayButton["button_play"]["position.x"].get<float>(), displayButton["button_play"]["position.y"].get<float>()},
                             displayButton["button_play"]["width"].get<float>(),
                             displayButton["button_play"]["height"].get<float>());

  buttonExit   = Rectangle({displayButton["button_exit"]["position.x"].get<float>(), displayButton["button_exit"]["position.y"].get<float>()},
                             displayButton["button_exit"]["width"].get<float>(),
                             displayButton["button_exit"]["height"].get<float>());

  buttonYes    = Rectangle({displayButton["button_yes"]["position.x"].get<float>(), displayButton["button_yes"]["position.y"].get<float>()},
                             displayButton["button_yes"]["width"].get<float>(),
                             displayButton["button_yes"]["height"].get<float>());

  buttonNo     = Rectangle({displayButton["button_no"]["position.x"].get<float>(), displayButton["button_no"]["position.y"].get<float>()},
                             displayButton["button_no"]["width"].get<float>(),
                             displayButton["button_no"]["height"].get<float>());
}
