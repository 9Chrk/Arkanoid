#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>
#include <allegro5/mouse.h>
#include <allegro5/system.h>
#include <allegro5/timer.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "model/utils.hpp"

static const ALLEGRO_COLOR WHITE   = al_map_rgb(255, 255, 255);
static const ALLEGRO_COLOR GREY    = al_map_rgb(192, 192, 192);
static const ALLEGRO_COLOR BLACK   = al_map_rgb(0,   0,   0);
static const ALLEGRO_COLOR YELLOW  = al_map_rgb(255, 223, 56);
static const ALLEGRO_COLOR PINK    = al_map_rgb(255, 114, 192);
static const ALLEGRO_COLOR BLUE    = al_map_rgb(122, 170, 255);
static const ALLEGRO_COLOR GREEN   = al_map_rgb(136, 196, 38);
static const ALLEGRO_COLOR ORANGE  = al_map_rgb(252, 104, 64);
static const ALLEGRO_COLOR RED     = al_map_rgb(255, 7, 69);
static const ALLEGRO_COLOR CYAN    = al_map_rgb(71, 255, 191);
static const ALLEGRO_COLOR MAGENTA = al_map_rgb(235, 37, 174);
static const ALLEGRO_COLOR SILVER  = al_map_rgb(224, 224, 224);
static const ALLEGRO_COLOR GOLD    = al_map_rgb(255, 215, 0);

inline constexpr double refreshPerSecond = 60;  

// Resource initialization
void must_init(bool test, const string& description);
void must_init(void* test, const string& description);

// Allegro error handling 
void checkBitmap(ALLEGRO_BITMAP* bitmap, const std::string& fileName); 
void checkSample(ALLEGRO_SAMPLE* sample, const std::string& fileName); 

// Allegro resource loading
ALLEGRO_BITMAP* loadBitmap(const std::string& filename);                     
ALLEGRO_SAMPLE* loadSample(const std::string& filename);                     

// Color utility function
ALLEGRO_COLOR getColor(const std::string& colorName); 
