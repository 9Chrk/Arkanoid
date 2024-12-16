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
#include "../libs/json.hpp"

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

using namespace std;
using json = nlohmann::json;

static const int    windowWidth      = 550;
static const int    windowHeight     = 500;
static const double refreshPerSecond = 60;

static const ALLEGRO_COLOR WHITE   = al_map_rgb(255, 255, 255);
static const ALLEGRO_COLOR GREY    = al_map_rgb(192, 192, 192);
static const ALLEGRO_COLOR BLACK   = al_map_rgb(50, 50, 50);
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


ALLEGRO_COLOR getColor(string colorName);
json openJsonFile(string fileName);
json readJsonFile(string fileName, string key);
void writeJsonFile(string fileName, string key, json value);

#endif
