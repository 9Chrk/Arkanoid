#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <utility>
#include <cstdio>
#include <memory>
#include <limits>
#include <algorithm>

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

#include "bonus_type.hpp"
#include "../libs/json.hpp"

using json = nlohmann::json;

static const int    windowWidth      = 550;
static const int    windowHeight     = 500;
static const double refreshPerSecond = 60;
static const float  playableArea     = 1.0f/3.0f;
extern ALLEGRO_FONT* font_bonus; // defined in main.cpp TEMPORARILY

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


// Colors and Bonus
ALLEGRO_COLOR getColor(const std::string& colorName);
std::string getBonusAbbreviation(const BonusType& bonusName);
BonusType getBonusType(const std::string& bonusName);

// JSON functions
json  openJsonFile(const std::string& fileName);
json  readJsonFile(const std::string& fileName, const std::string& key);
void writeJsonFile(const std::string& fileName, const std::string& key, const json& value);
void writeJsonFile(const std::string& fileName, const json& data);

// System commands
std::vector<std::string> executeCommand(const std::string& command);

// Allegro resource checking and loading
int checkBitmap(ALLEGRO_BITMAP* bitmap, const std::string& fileName);
int checkSample(ALLEGRO_SAMPLE* sample, const std::string& fileName);
ALLEGRO_BITMAP* loadBitmap(const char* filename);
ALLEGRO_SAMPLE* loadSample(const char* filename);

// Allegro resource initialization
void must_init(bool test, const char* description);
void must_init(void* test, const char* description);

#endif
