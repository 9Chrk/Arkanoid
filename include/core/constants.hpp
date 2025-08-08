#pragma once
#include "Rectangle.hpp"


inline constexpr float GAME_WIDTH      = 550.f;
inline constexpr float GAME_HEIGHT     = 500.f;

inline constexpr float PLAYABLE_RATIO  = 1.0f / 3.0f;
inline constexpr float PAUSE_SECONDS   = 4.0f;
inline constexpr float FRAME_THICKNESS = 1.0f;

inline constexpr int   SECOND_LIFE     = 200;
inline constexpr int   EMPTY_BRICK     = -1;
inline constexpr int   GOLD_BRICK      = 0;

inline constexpr Point INITIAL_DIRECTION = {0, -1};
enum class Direction { Left = -1, Right = 1 };
