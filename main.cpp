#include "include/common.hpp"
#include "include/games.hpp"


// ---- fonctions ----

void display_image(ALLEGRO_BITMAP* image_name) {
  al_clear_to_color(WHITE);
  al_draw_bitmap(image_name, 0, 0, 0);
  al_flip_display();
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

bool check_direction_changed (Point current_direction, Point &temp_direction) {
  if (current_direction.x != temp_direction.x || current_direction.y != temp_direction.y) {
    temp_direction = current_direction;
    return true;
  }
  return false;
}

void menu_button(ALLEGRO_BITMAP* start_png, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_SAMPLE* sample, ALLEGRO_SAMPLE* sound_menu,
                 bool &done, Rectangle button_on, Rectangle button_off) {    
  bool click_on_button = false;
  bool hover_play = false;
  bool hover_exit = false;
  Point click_pos;

  display_image(start_png);
  al_flip_display();

  ALLEGRO_SAMPLE_ID sound_menu_id;
  al_play_sample(sound_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sound_menu_id);

  while (!click_on_button) {
    al_flush_event_queue(queue);
    al_wait_for_event(queue, &event);
    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
      Point mouse_pos = {static_cast<float>(event.mouse.x), static_cast<float>(event.mouse.y)};
      bool new_hover_play = button_on.contains(mouse_pos);
      bool new_hover_exit = button_off.contains(mouse_pos);

      if (new_hover_play != hover_play || new_hover_exit != hover_exit) {
        hover_play = new_hover_play;
        hover_exit = new_hover_exit;

        display_image(start_png);
        if (hover_play) {
          pair<Point, Point> temp = button_on.diag_coor();
          al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          al_draw_rectangle(temp.first.x, temp.first.y, temp.second.x, temp.second.y, WHITE, 1);
        }
        if (hover_exit) {
          pair<Point, Point> temp = button_off.diag_coor();
          al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          al_draw_rectangle(temp.first.x, temp.first.y, temp.second.x, temp.second.y, WHITE, 1);
        }
        al_flip_display();
      }
    }
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      click_pos = {static_cast<float>(event.mouse.x), static_cast<float>(event.mouse.y)};
      if (button_on.contains(click_pos) || button_off.contains(click_pos)) {
        click_on_button = true;
        done = (button_on.contains(click_pos)) ? false : true;
      }
    }
  }
  al_stop_sample(&sound_menu_id);
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
  must_init(al_install_audio(), "audio");
  must_init(al_init_image_addon(), "image");
  must_init(al_init_font_addon(), "font_addon");
  must_init(al_init_ttf_addon(), "ttf_addon");
  must_init(al_init_primitives_addon(), "primitives");
  must_init(al_init_acodec_addon(), "acodec");
  must_init(al_reserve_samples(25), "acodec");

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

  ALLEGRO_FONT* font = al_load_font("./font/Distant_galaxy.ttf", 20, 0);
  must_init(font, "font");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  // images
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

  //sounds
  ALLEGRO_SAMPLE* bip_wav = al_load_sample("./sounds/bip.wav");
  ALLEGRO_SAMPLE* bonus_wav = al_load_sample("./sounds/bonus.wav");
  ALLEGRO_SAMPLE* button_wav = al_load_sample("./sounds/button.wav");
  ALLEGRO_SAMPLE* fall_wav = al_load_sample("./sounds/fall.wav");
  ALLEGRO_SAMPLE* lose_wav = al_load_sample("./sounds/lose.wav");
  ALLEGRO_SAMPLE* Street_Fighter_wav = al_load_sample("./sounds/Street_Fighter.wav");
  ALLEGRO_SAMPLE* win_wav = al_load_sample("./sounds/win.wav");
  ALLEGRO_SAMPLE* menu_wav = al_load_sample("./sounds/menu.wav");


  //// INITIALISATION VARIABLES ////

  // json //
  json display_score = readJsonFile("./data/settings.json", "display_score");
  json button = readJsonFile("./data/settings.json", "display_button");
  //
  float score_x = display_score["score.x"].get<float>();
  float score_y = display_score["score.y"].get<float>();
  float highscore_x = display_score["highscore.x"].get<float>();
  float highscore_y = display_score["highscore.y"].get<float>();
  //
  float button_play_position_x = button["button_play"]["position.x"].get<float>();
  float button_play_position_y = button["button_play"]["position.y"].get<float>();
  float button_play_width = button["button_play"]["width"].get<float>();
  float button_play_height = button["button_play"]["height"].get<float>();
  ALLEGRO_COLOR button_play_color = getColor(button["button_play"]["color"].get<string>());
  //
  float button_exit_position_x = button["button_exit"]["position.x"].get<float>();
  float button_exit_position_y = button["button_exit"]["position.y"].get<float>();
  float button_exit_width = button["button_exit"]["width"].get<float>();
  float button_exit_height = button["button_exit"]["height"].get<float>();
  ALLEGRO_COLOR button_exit_color = getColor(button["button_exit"]["color"].get<string>());
  //
  float button_yes_position_x = button["button_yes"]["position.x"].get<float>();
  float button_yes_position_y = button["button_yes"]["position.y"].get<float>();
  float button_yes_width = button["button_yes"]["width"].get<float>();
  float button_yes_height = button["button_yes"]["height"].get<float>();
  ALLEGRO_COLOR button_yes_color = getColor(button["button_yes"]["color"].get<string>());
  //
  float button_no_position_x = button["button_no"]["position.x"].get<float>();
  float button_no_position_y = button["button_no"]["position.y"].get<float>();
  float button_no_width = button["button_no"]["width"].get<float>();
  float button_no_height = button["button_no"]["height"].get<float>();
  ALLEGRO_COLOR button_no_color = getColor(button["button_no"]["color"].get<string>());
  //////////

  bool           done;
  bool    restartGame; 
  ALLEGRO_EVENT event;
  Games          game;

  Point score_pos({score_x, score_y});
  Point highscore_pos({highscore_x, highscore_y});
  Point temp_direction = game.ball.getDirection();

  Rectangle button_play = Rectangle({button_play_position_x, button_play_position_y},
                                     button_play_width, button_play_height, 
                                     button_play_color, button_play_color);
                                     
  Rectangle button_exit = Rectangle({button_exit_position_x, button_exit_position_y},
                                     button_exit_width, button_exit_height, 
                                     button_exit_color, button_exit_color);

  Rectangle button_yes = Rectangle({button_yes_position_x, button_yes_position_y},
                                    button_yes_width, button_yes_height, 
                                    button_yes_color, button_yes_color);

  Rectangle button_no = Rectangle({button_no_position_x, button_no_position_y},
                                   button_no_width, button_no_height, 
                                   button_no_color, button_no_color);


  /////////////////   GAME   /////////////////

  menu_button(start_png, event, queue, button_wav, menu_wav, done, button_play, button_exit);

  ALLEGRO_SAMPLE_ID sound_game_id;
  al_play_sample(Street_Fighter_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sound_game_id);

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
        // Vérifie si collision
        if (check_direction_changed(game.ball.getDirection(), temp_direction)) {
          al_play_sample(bip_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        };
        game.checkCollisions(); // entre la balle et les briques
        // Vérifie si la balle est tombée
        if (game.ball.isFalling()) {
          game.spaceship.damage();
          game.ball.reset();
          game.ball.setFalling(false);
          if (game.spaceship.getHealth() > 0) {
            al_play_sample(fall_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          }
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
          al_stop_sample(&sound_game_id);
          al_play_sample(lose_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          cout << "\nGame Over... Les briques ont gagné cette fois-ci. 🧱" << endl;
          cout << "Score atteint : " << game.getScore() << "\n" << endl;
          done = true;
          al_rest(5.0);
        } else if (game.win()) {
          display_image(win_png);
          al_stop_sample(&sound_game_id);
          al_play_sample(win_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          cout << "\nFélicitations ! Tu as brisé toutes les briques ! 🎇" << endl;
          cout << "Score atteint : " << game.getScore() << "\n" << endl;
          done = true;
          al_rest(5.0);
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
  game.saveHighScore();
  // les ressources allouées dynamiquement sont détruites
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  // images
  al_destroy_bitmap(start_png);
  al_destroy_bitmap(background_png);
  al_destroy_bitmap(win_png);
  al_destroy_bitmap(lose_png);
  al_destroy_bitmap(spaceship_png);
  // sounds
  al_destroy_sample(win_wav);
  al_destroy_sample(fall_wav);
  al_destroy_sample(button_wav);
  al_destroy_sample(Street_Fighter_wav);
  al_destroy_sample(bonus_wav);
  al_destroy_sample(bip_wav);
  al_destroy_sample(menu_wav);
  al_uninstall_audio();

  return 0;
}
