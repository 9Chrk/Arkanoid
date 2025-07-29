#pragma once
#include "view/utils.hpp"
#include <memory>


struct AllegroConfig {
  using BitmapPtr = std::unique_ptr<ALLEGRO_BITMAP, decltype(&al_destroy_bitmap)>;
  using FontPtr   = std::unique_ptr<ALLEGRO_FONT,   decltype(&al_destroy_font)>;
  using SamplePtr = std::unique_ptr<ALLEGRO_SAMPLE, decltype(&al_destroy_sample)>;

  ALLEGRO_EVENT event;
  // components
  ALLEGRO_TIMER* timer     = nullptr;
  ALLEGRO_DISPLAY* display = nullptr;
  FontPtr    font{nullptr, al_destroy_font};
  FontPtr    fontBonus{nullptr, al_destroy_font};
  ALLEGRO_EVENT_QUEUE* queue = nullptr;
  // images
  BitmapPtr start_png{nullptr, al_destroy_bitmap};
  BitmapPtr background_png{nullptr, al_destroy_bitmap};
  BitmapPtr lose_png{nullptr, al_destroy_bitmap};
  BitmapPtr win_png{nullptr, al_destroy_bitmap};
  BitmapPtr spaceship_png{nullptr, al_destroy_bitmap};
  BitmapPtr heart_1_png{nullptr, al_destroy_bitmap};
  BitmapPtr heart_2_png{nullptr, al_destroy_bitmap};
  BitmapPtr heart_3_png{nullptr, al_destroy_bitmap};
  BitmapPtr score_png{nullptr, al_destroy_bitmap};
  BitmapPtr highScore_png{nullptr, al_destroy_bitmap};
  BitmapPtr finish_png{nullptr, al_destroy_bitmap};
  // sounds
  SamplePtr bip_wav{nullptr, al_destroy_sample};
  SamplePtr bonus_wav{nullptr, al_destroy_sample};
  SamplePtr button_wav{nullptr, al_destroy_sample};
  SamplePtr fall_wav{nullptr, al_destroy_sample};
  SamplePtr lose_wav{nullptr, al_destroy_sample};
  SamplePtr Street_Fighter_wav{nullptr, al_destroy_sample};
  SamplePtr win_wav{nullptr, al_destroy_sample};
  SamplePtr menu_wav{nullptr, al_destroy_sample};
  SamplePtr finish_wav{nullptr, al_destroy_sample};

  AllegroConfig();
  ~AllegroConfig();

  // Forbid copying and assignment
  AllegroConfig(const AllegroConfig&) = delete;
  AllegroConfig& operator=(const AllegroConfig&) = delete;
};
