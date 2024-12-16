#include "include/common.hpp"
#include "include/games.hpp"


void display_image(ALLEGRO_BITMAP* image_name) {
  al_clear_to_color(WHITE);
  al_draw_bitmap(image_name, 0, 0, 0);
  al_flip_display();
  al_rest(2.0);
}

void display_info_game(ALLEGRO_BITMAP* score_png, ALLEGRO_BITMAP* highScore_png, ALLEGRO_BITMAP* heart_png, ALLEGRO_FONT* font, Games game,
                       Point score_pos, Point highscore_pos) {
  al_draw_bitmap(score_png, 0, 0, 0);
  al_draw_bitmap(highScore_png, windowWidth * 0, 0, 0);
  al_draw_bitmap(heart_png, windowWidth * 0, 0, 0);
  al_draw_text(font, WHITE, score_pos.x, score_pos.y, ALLEGRO_ALIGN_CENTER, to_string(game.getScore()).c_str());
  al_draw_text(font, WHITE, highscore_pos.x, highscore_pos.y, ALLEGRO_ALIGN_CENTER, to_string(game.getHighScore()).c_str());
}

void display_spaceship(ALLEGRO_BITMAP* spaceship_png, Games& game) {
  Point spaceship_pos = game.spaceship.getPosition();
  float spaceship_width = game.spaceship.getWidth();
  float spaceship_height = game.spaceship.getHeight();
  al_draw_bitmap(spaceship_png, spaceship_pos.x - spaceship_width/2 - 3, spaceship_pos.y - spaceship_height/2 - 1, 0);
}

ALLEGRO_BITMAP* choose_heartFile(ALLEGRO_BITMAP* heart_1_png, ALLEGRO_BITMAP* heart_2_png, ALLEGRO_BITMAP* heart_3_png, Games& game) {
  int health = game.spaceship.getHealth();
  if (health == 1) { return heart_1_png; }
  else if (health == 2) { return heart_2_png; }
  else { return heart_3_png; }
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
  must_init(al_init_image_addon(), "image");
  must_init(al_init_font_addon(), "font_addon");
  must_init(al_init_ttf_addon(), "ttf_addon");
  must_init(al_init_primitives_addon(), "primitives");

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / refreshPerSecond);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  ALLEGRO_DISPLAY* disp = al_create_display(windowWidth, windowHeight);
  must_init(disp, "display");

  al_set_window_title(disp, "Arkanoid");

  ALLEGRO_FONT* font = al_load_font("./font/Distant_galaxy.ttf", 20, NULL);
  must_init(font, "font");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_timer_event_source(timer));

  ALLEGRO_BITMAP* start_png = al_load_bitmap("./image/start.png");
  ALLEGRO_BITMAP* background_png = al_load_bitmap("./image/background.png");
  ALLEGRO_BITMAP* lose_png = al_load_bitmap("./image/lose.png");
  ALLEGRO_BITMAP* win_png = al_load_bitmap("./image/win.png");
  ALLEGRO_BITMAP* spaceship_png = al_load_bitmap("./image/spaceship.png");
  ALLEGRO_BITMAP* heart_1_png = al_load_bitmap("./image/1heart.png");
  ALLEGRO_BITMAP* heart_2_png = al_load_bitmap("./image/2heart.png");
  ALLEGRO_BITMAP* heart_3_png = al_load_bitmap("./image/3heart.png");
  ALLEGRO_BITMAP* score_png = al_load_bitmap("./image/score.png");
  ALLEGRO_BITMAP* highScore_png = al_load_bitmap("./image/high_score.png");

  //// GESTION DU CODE ////

  json display_score = readJsonFile("./data/settings.json", "display_score");
  Point score_pos({display_score["score.x"].get<float>(), display_score["score.y"].get<float>()});
  Point highscore_pos({display_score["highscore.x"].get<float>(), display_score["highscore.y"].get<float>()});
  
  bool          done = false;
  ALLEGRO_EVENT event;
  Games         game;

  al_start_timer(timer);
  display_image(start_png);

  while (!done) {
    al_wait_for_event(queue, &event);
    switch (event.type) {
      case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT
         || event.keyboard.keycode == ALLEGRO_KEY_A
         || event.keyboard.keycode == ALLEGRO_KEY_Q) {
          game.spaceship.move(0); // Gauche
        } 
        else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT
              || event.keyboard.keycode == ALLEGRO_KEY_D
              || event.keyboard.keycode == ALLEGRO_KEY_P) {
          game.spaceship.move(1); // Droite
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
          game.ball.setMouvement(true);
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
          game.ball.reset();
          game.resetHighScore();
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
          done = true;
        }
        break;
      case ALLEGRO_EVENT_MOUSE_AXES: {
        game.spaceship.move({static_cast<float>(event.mouse.x),
                             static_cast<float>(event.mouse.y)});
        break;
      }
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
      case ALLEGRO_EVENT_TIMER: {
        // Vérifie si collision entre la balle et les briques
        game.checkCollisions();
        // Vérifie si la balle est tombée
        if (game.ball.isFalling()) {
          game.spaceship.damage();
          game.ball.reset();
          game.ball.setFalling(false);
        } else {
          // Met la balle en mouvement (lancement/lancée)
          if (game.ball.inMouvement()) {
            game.ball.move(game.spaceship.getPosition(),
                           game.spaceship.getWidth(),
                           game.spaceship.getHeight());
          } else {
            game.ball.move(game.spaceship.getPosition());
          }
        }
        // Vérification Fin de partie (Win/Loose)
        if (game.loose()) {
          display_image(lose_png);
          cout << "\nGame Over... Les briques ont gagné cette fois-ci. 🧱" << endl;
          cout << "Score atteint : " << game.getScore() << "\n" << endl;
          done = true;
        } else if (game.win()) {
          display_image(win_png);
          cout << "\nFélicitations ! Tu as brisé toutes les briques ! 🎇" << endl;
          cout << "Score atteint : " << game.getScore() << "\n" << endl;
          done = true;
        } else {
          // Affichage
          al_clear_to_color(WHITE);
          al_draw_bitmap(background_png, 0, 0, 0);
          ALLEGRO_BITMAP* heart_png = choose_heartFile(heart_1_png, heart_2_png, heart_3_png, game);
          display_info_game(score_png, highScore_png, heart_png, font, game, score_pos, highscore_pos); 
          game.draw();
          display_spaceship(spaceship_png, game);
          al_flip_display();
          break;
        }
      }
      default: {
        break;
      }
    }
  }
  // retient le meilleur score
  game.saveHighScore();
  // les ressources allouées dynamiquement sont détruites
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  al_destroy_bitmap(start_png);
  al_destroy_bitmap(background_png);
  al_destroy_bitmap(win_png);
  al_destroy_bitmap(lose_png);
  al_destroy_bitmap(spaceship_png);

  return 0;
}
