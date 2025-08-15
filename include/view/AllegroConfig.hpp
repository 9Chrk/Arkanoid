#pragma once
#include "utils.hpp"


struct AllegroConfig {
  ALLEGRO_EVENT event;
  // components
  ALLEGRO_TIMER* timer                = nullptr;
  ALLEGRO_DISPLAY* display            = nullptr;
  ALLEGRO_FONT* font                  = nullptr;
  ALLEGRO_FONT* fontBonus             = nullptr;
  ALLEGRO_EVENT_QUEUE* queue          = nullptr;
  // images
  ALLEGRO_BITMAP* start_png           = nullptr;
  ALLEGRO_BITMAP* background_png      = nullptr;
  ALLEGRO_BITMAP* lose_png            = nullptr;
  ALLEGRO_BITMAP* win_png             = nullptr;
  ALLEGRO_BITMAP* spaceship_png       = nullptr;
  ALLEGRO_BITMAP* heart_1_png         = nullptr;
  ALLEGRO_BITMAP* heart_2_png         = nullptr;
  ALLEGRO_BITMAP* heart_3_png         = nullptr;
  ALLEGRO_BITMAP* score_png           = nullptr;
  ALLEGRO_BITMAP* highScore_png       = nullptr;
  ALLEGRO_BITMAP* finish_png          = nullptr;
  // sounds
  ALLEGRO_SAMPLE* bip_wav             = nullptr;
  ALLEGRO_SAMPLE* bonus_wav           = nullptr;
  ALLEGRO_SAMPLE* button_wav          = nullptr;
  ALLEGRO_SAMPLE* fall_wav            = nullptr;
  ALLEGRO_SAMPLE* lose_wav            = nullptr;
  ALLEGRO_SAMPLE* street_Fighter_wav  = nullptr;
  ALLEGRO_SAMPLE* win_wav             = nullptr;
  ALLEGRO_SAMPLE* menu_wav            = nullptr;
  ALLEGRO_SAMPLE* finish_wav          = nullptr;

  AllegroConfig();
  ~AllegroConfig();

  // Forbid copying and assignment
  AllegroConfig(const AllegroConfig&) = delete;
  AllegroConfig& operator=(const AllegroConfig&) = delete;
};
