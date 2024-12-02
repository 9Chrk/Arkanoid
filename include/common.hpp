#ifndef COMMON_HPP
#define COMMON_HPP

#include <allegro5/allegro5.h>
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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <vector>
#include <utility>

using namespace std;

const int    windowWidth      = 500;
const int    windowHeight     = 500;
const double refreshPerSecond = 60;

const ALLEGRO_COLOR WHITE   = al_map_rgb(255, 255, 255);
const ALLEGRO_COLOR GREY    = al_map_rgb(128, 128, 128);
const ALLEGRO_COLOR BLACK   = al_map_rgb(0, 0, 0);
const ALLEGRO_COLOR ORANGE  = al_map_rgb(255, 165, 0);  
const ALLEGRO_COLOR CYAN    = al_map_rgb(0, 255, 255);  
const ALLEGRO_COLOR GREEN   = al_map_rgb(0, 255, 0);    
const ALLEGRO_COLOR RED     = al_map_rgb(255, 0, 0);    
const ALLEGRO_COLOR BLUE    = al_map_rgb(0, 0, 255);    
const ALLEGRO_COLOR MAGENTA = al_map_rgb(255, 0, 255);  
const ALLEGRO_COLOR YELLOW  = al_map_rgb(255, 255, 0);

#endif