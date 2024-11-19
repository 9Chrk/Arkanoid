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
const ALLEGRO_COLOR ORANGE  = al_map_rgb(255, 165, 0);  
const ALLEGRO_COLOR CYAN    = al_map_rgb(0, 255, 255);  
const ALLEGRO_COLOR GREEN   = al_map_rgb(0, 255, 0);    
const ALLEGRO_COLOR RED     = al_map_rgb(255, 0, 0);    
const ALLEGRO_COLOR BLUE    = al_map_rgb(0, 0, 255);    
const ALLEGRO_COLOR MAGENTA = al_map_rgb(255, 0, 255);  
const ALLEGRO_COLOR YELLOW  = al_map_rgb(255, 255, 0);  

// NIVEAU 1
vector<int> scores = {50, 90, 120, 100, 110, 80};
vector<ALLEGRO_COLOR> colors = {WHITE, RED, YELLOW, BLUE, MAGENTA, GREEN};

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
            ALLEGRO_COLOR frameColor = COLOR_BLACK,
            ALLEGRO_COLOR fillColor = COLOR_WHITE);

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

class Cell {
  Rectangle r;
 public:
  Cell(Point center, float w, float h);
  void draw();
  void mouseMove(Point mouseLoc);
  void keySpace(int keyCode);
};

Cell::Cell(Point center, float w, float h) : r(center, w, h, COLOR_BLACK, COLOR_WHITE) {}

void Cell::draw() {
  r.draw();
}

void Cell::mouseMove(Point mouseLoc) {
}

void Cell::keySpace(int keyCode) {
}


// ---- Création du grillage de rectangle ----

class Canvas {
  vector<Cell> cells{};

 public:
  Canvas();
  void draw();
  void mouseMove(Point mouseLoc);
  void keySpace(int keycode);
};

Canvas::Canvas() {
  const int dim = 10;
  const int size = 40;
  const int margin = 10;
  const int offset = size + margin;

  for (int i=0; i < dim; ++i) {
    for (int j=0; j < dim; ++j) {
      cells.push_back(Cell({offset*i + offset/2, offset*j + offset/2}, size, size));
    }
  }
}

void Canvas::draw() {
  for (auto& cell : cells) {
    cell.draw();
  }
}

void Canvas::mouseMove(Point mouseLoc) {
}

void Canvas::keySpace(int keycode){
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
  Canvas        canvas;

  al_start_timer(timer);
  while (!done) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
      case ALLEGRO_KEY_SPACE:
        canvas.keySpace(event.keyboard.keycode);
        break;
      case ALLEGRO_EVENT_MOUSE_AXES:
        canvas.mouseMove({static_cast<float>(event.mouse.x),
                          static_cast<float>(event.mouse.y)});
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
      case ALLEGRO_EVENT_TIMER:
        al_clear_to_color(al_map_rgb(255, 255, 255));
        canvas.draw();
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