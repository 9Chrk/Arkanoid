#pragma once
#include "Rectangle.hpp"


inline constexpr float  GAME_WIDTH  = 550.f;
inline constexpr float  GAME_HEIGHT = 500.f;
inline constexpr double REFRESH_PER_SECOND = 60;

inline constexpr int   FONT_SIZE              = 20;
inline constexpr int   FONT_BONUS_SIZE        = 8;
inline constexpr int   AUDIO_RESERVED_SAMPLES = 32;
inline constexpr int   ANTIALIASING_SAMPLES   = 8;

inline constexpr float OUT_OF_BOUNDS_Y_FACTOR = 1.5f;
inline constexpr float PLAYABLE_RATIO  = 1.0f / 3.0f;
inline constexpr float PAUSE_SECONDS   = 4.0f;
inline constexpr float FRAME_THICKNESS = 1.0f;

inline constexpr int   SECOND_LIFE     = 200;
inline constexpr int   EMPTY_BRICK     = -1;
inline constexpr int   GOLD_BRICK      = 0;

inline constexpr float BOUNCE_MIN_DEG = 30.0f;
inline constexpr float BOUNCE_RANGE_DEG = 120.0f;
inline constexpr float BOUNCE_MAX_DEG = 150.0f;

inline constexpr float EXPAND_WIDTH_FACTOR     = 1.5f;
inline constexpr float EXPAND_TIMER_DURATION   = 50.0f;
inline constexpr float CATCH_RELEASE_DURATION  = 10.0f;
inline constexpr float TIMER_DECREMENT         = 0.1f;
inline constexpr float SLOW_DOWN_FACTOR        = 0.8f;
inline constexpr float BONUS_TEXT_VERTICAL_OFFSET = 2.0f;

inline constexpr Point INITIAL_DIRECTION = {0, -1};
enum class Direction { Left = -1, Right = 1 };
