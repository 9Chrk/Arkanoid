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


// ---------------------------------------------------------------------------------------------------------

struct Point {
  float x = 0, y = 0;
};

// ---- Création des Rectangles ----

class Rectangle {
 private:
  Point center;
  float w;
  float h;
  ALLEGRO_COLOR frameColor;
  ALLEGRO_COLOR fillColor;
  pair<Point, Point> diag_coor();

 public:
  Rectangle(Point center, float w, float h, 
            ALLEGRO_COLOR frameColor = BLACK,
            ALLEGRO_COLOR fillColor = WHITE);

  void draw();
  void setFillColor(ALLEGRO_COLOR newFillColor);
  void setFrameColor(ALLEGRO_COLOR newFrameColor);
  bool contains(Point p);
};

Rectangle::Rectangle(Point center, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor)
    : center(center), w(w), h(h), frameColor(frameColor), fillColor(fillColor) {}

pair<Point, Point> Rectangle::diag_coor(){
  Point up_left = {center.x - w/2, center.y - h/2};
  Point down_right = {center.x + w/2, center.y + h/2};
  return make_pair(up_left, down_right);
}

void Rectangle::draw() {
  pair<Point, Point> coord = diag_coor();
  al_draw_filled_rectangle(coord.first.x, coord.first.y, coord.second.x, coord.second.y, fillColor);
  al_draw_rectangle(coord.first.x, coord.first.y, coord.second.x, coord.second.y, frameColor, 1);
}

bool Rectangle::contains(Point p) {
    std::pair<Point, Point> coord = diag_coor();
    return (coord.first.x <= p.x && p.x <= coord.second.x &&
            coord.first.y <= p.y && p.y <= coord.second.y);
}

void Rectangle::setFillColor(ALLEGRO_COLOR newFillColor){
  fillColor = newFillColor;
}

void Rectangle::setFrameColor(ALLEGRO_COLOR newFrameColor){
  frameColor = newFrameColor;
}


// ---- Fonctionnalités liées aux rectangles ----

class Brick : public Rectangle {
 private:
  int scores;
  bool destroyed;
  
 public:
  Brick(Point center, float w, float h, 
       ALLEGRO_COLOR frameColor, 
       ALLEGRO_COLOR fillColor, 
       int scores);
  
  int destroy() {destroyed = true; return scores;};
};

Brick::Brick(Point center, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor, int scores) 
    : Rectangle(center, w, h, frameColor, fillColor), scores(scores), destroyed(false) {}


// ---- Création du grillage de brique ----

class Games {
 private:
  vector<Brick> bricks{};

 public:
  Games();
  void draw();
};

Games::Games() {

  // NIVEAU 1
  vector<int> scores = {50, 90, 120, 100, 110, 80};
  vector<ALLEGRO_COLOR> colors = {GREY, RED, YELLOW, BLUE, MAGENTA, GREEN};
  const int dim_x = 13;
  const int dim_y = 6;

  // brick spec
  const int width = 30;
  const int height = 15;
  const int margin = 3;

  const int offset_x = width + margin;
  const int offset_y = height + margin;
  const int total_width = dim_x * width + (dim_x - 1) * margin;
  const int offset_start_x = (500 - total_width) / 2;
  const int offset_start_y = 0;

  for (int i = 0; i < dim_x; ++i) {
      for (int j = 0; j < dim_y; ++j) {
          bricks.push_back(Brick(
              {offset_start_x + offset_x * i + offset_x / 2, 
              offset_start_y + offset_y * j + offset_y / 2},
              width, height, BLACK, colors.at(j), scores.at(j)));
      }
  }
}

void Games::draw() {
  for (auto& brick : bricks) {
    brick.draw();
  }
}


// ---- fonctions de test ----

void must_init(bool test, const char* description) {
  if (test) {
    return;
  }
  std::cerr << "couldn't initialize " << description << '\n';
  exit(1);
}

void must_init(void* test, const char* description) {
  must_init(test != nullptr, description);
}


// ---------------------------- MAIN ----------------------------

int main(int /* argc */, char** /* argv */) {

  //// INITIALISATION ALLEGRO //// 

  must_init(al_init(), "allegro");
  must_init(al_install_keyboard(), "keyboard");
  must_init(al_install_mouse(), "mouse");

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / refreshPerSecond);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  ALLEGRO_DISPLAY* disp = al_create_display(windowWidth, windowHeight);
  must_init(disp, "display");

  ALLEGRO_FONT* font = al_create_builtin_font();
  must_init(font, "font");

  must_init(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_timer_event_source(timer));

  //// GESTION DU CODE ////
  
  bool          done = false;
  ALLEGRO_EVENT event;
  Games        game;

  al_start_timer(timer);
  while (!done) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
      case ALLEGRO_EVENT_KEY_DOWN:
        //game.keyDown(event.keyboard.keycode);
        break;
      case ALLEGRO_EVENT_MOUSE_AXES:
        //game.mouseMove({static_cast<float>(event.mouse.x),
        //                  static_cast<float>(event.mouse.y)});
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
      case ALLEGRO_EVENT_TIMER:
        al_clear_to_color(al_map_rgb(255, 255, 255));
        game.draw();
        al_flip_display();
        break;
      default: {}
    }
  }
  // les ressources allouées dynamiquement sont détruites
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}