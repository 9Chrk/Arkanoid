#pragma once
#include "core/utils.hpp"


enum class InputActionType {
  MoveLeft, MoveRight, // Keyboard movement
  Launch,              // Launch the ball
  Reset,               // Reset level stats
  Quit,                // Quit app
  TimerTick,           // Timer tick
  MouseMove,           // Mouse movement
  LevelChange          // Cheating
};

struct InputAction {
    InputActionType type;
    // Action data
    int x = 0;
    int y = 0;
};
