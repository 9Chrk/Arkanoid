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

// --------------

class Rectangle {
 protected:
  Point position;
  float w, h;
  ALLEGRO_COLOR frameColor, fillColor;
  pair<Point, Point> diag_coor();

 public:
  Rectangle(Point position, float w, float h, 
            ALLEGRO_COLOR frameColor = BLACK,
            ALLEGRO_COLOR fillColor = WHITE);

  void draw();
  bool contains(Point p);
};

Rectangle::Rectangle(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor)
    : position(position), w(w), h(h), frameColor(frameColor), fillColor(fillColor) {}

pair<Point, Point> Rectangle::diag_coor(){
  Point up_left = {position.x - w/2, position.y - h/2};
  Point down_right = {position.x + w/2, position.y + h/2};
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

// --------------

class Brick : public Rectangle { // rajout des bonus plus tard
 private:
  int scores;
  bool destroyed;
  
 public:
  Brick(Point position, float w, float h, 
       ALLEGRO_COLOR frameColor, 
       ALLEGRO_COLOR fillColor, 
       int scores);
  
  int destroy() {destroyed = true; return scores;}
  bool isDestroyed() {return destroyed;}
};

Brick::Brick(Point position, float w, float h, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor, int scores) 
    : Rectangle(position, w, h, frameColor, fillColor), scores(scores), destroyed(false) {}

// --------------

class Ball {
 private:
  int rayon, vitesse;
  ALLEGRO_COLOR color;
  Point position, d;

 public:
  Ball(Point position, int rayon, int vitesse, ALLEGRO_COLOR color);

  void draw();
  void move();
  void move(Point position);
  void bounce();
  void reset();
  void fall();
  bool inMouvement, isFalling;
};

Ball::Ball(Point position, int rayon, int vitesse, ALLEGRO_COLOR color)
    : position(position), rayon(rayon), vitesse(vitesse), color(color), inMouvement(false), isFalling(false), d({0, -1}) {}

void Ball::draw() {
  al_draw_circle(position.x, position.y, rayon, color, rayon*2);
}

void Ball::move() {
  position.x += d.x * vitesse;
  position.y += d.y * vitesse;
  bounce();    // vérification collision
  fall();      // vérification chute
}

void Ball::move(Point Spaceship_Pos) {
  position.x = Spaceship_Pos.x;
  position.y = Spaceship_Pos.y - rayon*3.5;
}

void Ball::bounce() {
  // rebond avec les murs
  if (position.x - rayon <= 0 || position.x + rayon >= windowWidth) {d.x *= -1;};
  if (position.y - rayon <= 0) {d.y *= -1;};
}

void Ball::fall() {
  isFalling = (position.y - rayon > windowHeight) ? true : false;
}

void Ball::reset() {
  d = {0, -1};
  inMouvement = false;
}

// --------------

class Spaceship : public Rectangle {
 private:
  int health, vitesse;
  Point reset_pos;
 
 public:
  Spaceship(Point position, float w, float h, int vitesse, int health, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor);

  void move(int direction);
  void move(Point mousePosition);
  bool validPosition(Point position);
  Point getPosition() {return position;}
  void damage() {health--;}
  bool isDeath() {return health <= 0;}
  void reset() {position = reset_pos;}
};

Spaceship::Spaceship(Point position, float w, float h, int vitesse, int health, ALLEGRO_COLOR frameColor, ALLEGRO_COLOR fillColor) 
    : Rectangle(position, w, h, frameColor, fillColor), health(health), reset_pos(position), vitesse(vitesse) {}

void Spaceship::move(int direction) {
  Point newPos = position;
  newPos.x += (direction == 0 ? -vitesse : vitesse);

  if (validPosition(newPos)) {
    position = newPos;
  } else {
    // ATTENTION : fonction proposé par chatGPT pour limiter newPos a une plage de valeur
    position.x = clamp(newPos.x, w / 2.0f, windowWidth - w / 2.0f);
  }
}

void Spaceship::move(Point mousePosition) {
  Point newPos = position;
  newPos.x = mousePosition.x;
  if (validPosition(newPos) && mousePosition.y >= windowHeight*2/3) {position = newPos;}
}

bool Spaceship::validPosition(Point newPos) {
  return (newPos.x - w/2 >= 0) && (newPos.x + w/2 <= windowWidth);
}


// --------- classe principale ---------

class Games {
 private:
  vector<Brick> bricks{};

 public:
  Ball ball;           
  Spaceship spaceship;
  Games();
  void draw();
  bool win();
  bool loose();
};

Games::Games() 
    : ball({250, 453}, 5, 10, BLACK), 
      spaceship({250, 470}, 100, 15, 75, 3, BLACK, BLACK) {

  // Initialisation des briques
  vector<int> scores = {50, 90, 120, 100, 110, 80};
  vector<ALLEGRO_COLOR> colors = {GREY, RED, YELLOW, BLUE, MAGENTA, GREEN};
  const int dim_x = 13;
  const int dim_y = 6;

  // Spécifications des briques
  const int width = 35;
  const int height = 15;
  const int margin = 3;

  const int offset_x = width + margin;
  const int offset_y = height + margin;
  const int total_width = dim_x * width + (dim_x - 1) * margin;
  const int offset_start_x = (500 - total_width) / 2;
  const int offset_start_y = 20;

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
    if (!brick.isDestroyed()) {brick.draw();}
  }
  ball.draw();
  spaceship.draw();
}

bool Games::loose() {
  return spaceship.isDeath();
}

bool Games::win() {
  for (auto& brick : bricks) {
    if (!brick.isDestroyed()) {return false;}
  }
  return true;
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
  Games         game;

  al_start_timer(timer);
  while (!done) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
      case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
          game.spaceship.move(0); // Gauche
        } 
        else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
          game.spaceship.move(1); // Droite
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
          game.ball.inMouvement = true;
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
          game.spaceship.reset();
          game.ball.reset();
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
          done = true;
        }
        break;
      case ALLEGRO_EVENT_MOUSE_AXES:
        game.spaceship.move({static_cast<float>(event.mouse.x),
                             static_cast<float>(event.mouse.y)});
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
      case ALLEGRO_EVENT_TIMER:
        // Verifie si la balle est tombé
        if (game.ball.isFalling) {
          game.spaceship.damage();
          game.spaceship.reset();
          game.ball.reset();
          game.ball.isFalling = false;
        } 
        else {
          // Met la balle en mouvement (lancement/lancée)
          if (game.ball.inMouvement) {game.ball.move();}
          else {game.ball.move(game.spaceship.getPosition());}
        }
        // Verification Fin de partie (Win/Loose)
        if (game.loose() || game.win()) {
          cout << "Fin de la Partie..." << endl;
          done = true;
        }
        // Affichage
        al_clear_to_color(WHITE);
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