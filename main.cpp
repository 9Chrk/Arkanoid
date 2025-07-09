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

void forceChangeLevel(int index, int game_score, Games &game, Point &temp_direction, vector<string> game_levels) {
  game = Games(game_levels.at(index), game_score);
  temp_direction = game.ball.getDirection();
};

void menu(ALLEGRO_BITMAP* image_png, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_SAMPLE* sound_menu, ALLEGRO_FONT* font, char* text) {
  ALLEGRO_SAMPLE_ID sound_menu_id;
  bool press_any_button = false;

  display_image(image_png);
  al_play_sample(sound_menu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sound_menu_id);

  al_rest(4.0);
  float font_height = static_cast<float>(al_get_font_line_height(font));
  al_draw_text(font, WHITE, windowWidth * 1/3, windowHeight - font_height, ALLEGRO_ALIGN_CENTER, text);
  al_flip_display();

  while (!press_any_button) { 
    al_flush_event_queue(queue);
    al_wait_for_event(queue, &event);
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) { press_any_button = true; }
  }
  al_stop_sample(&sound_menu_id);
}

void menu_button(ALLEGRO_BITMAP* image_png, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_SAMPLE* sample, ALLEGRO_SAMPLE* sound_menu,
                 bool &done, Rectangle button_on, Rectangle button_off) {    
  bool click_on_button = false;
  bool hover_play = false;
  bool hover_exit = false;
  Point click_pos;

  display_image(image_png);
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

        display_image(image_png);
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
        done = (button_on.contains(click_pos)) ? true : false;
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
  cerr << "couldn't initialize " << description << '\n';
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
  must_init(al_init_acodec_addon(), "acodec");
  must_init(al_reserve_samples(32), "acodec");
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
  al_set_new_display_flags(ALLEGRO_WINDOWED);
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
  ALLEGRO_BITMAP* start_png = al_load_bitmap("./images/start.png");
  ALLEGRO_BITMAP* background_png = al_load_bitmap("./images/background.png");
  ALLEGRO_BITMAP* lose_png = al_load_bitmap("./images/lose.png");
  ALLEGRO_BITMAP* win_png = al_load_bitmap("./images/win.png");
  ALLEGRO_BITMAP* spaceship_png = al_load_bitmap("./images/spaceship.png");
  ALLEGRO_BITMAP* heart_1_png = al_load_bitmap("./images/1heart.png");
  ALLEGRO_BITMAP* heart_2_png = al_load_bitmap("./images/2heart.png");
  ALLEGRO_BITMAP* heart_3_png = al_load_bitmap("./images/3heart.png");
  ALLEGRO_BITMAP* score_png = al_load_bitmap("./images/score.png");
  ALLEGRO_BITMAP* highScore_png = al_load_bitmap("./images/high_score.png");
  ALLEGRO_BITMAP* finish_png = al_load_bitmap("./images/finish.png");

  // check if images are loaded correctly
  checkBitmap(start_png, "start.png");
  checkBitmap(background_png, "background.png");
  checkBitmap(lose_png, "lose.png");
  checkBitmap(win_png, "win.png");
  checkBitmap(heart_1_png, "1heart.png");
  checkBitmap(heart_2_png, "2heart.png");
  checkBitmap(heart_3_png, "3heart.png");
  checkBitmap(score_png, "score.png");
  checkBitmap(highScore_png, "high_score.png");
  checkBitmap(finish_png, "finish.png");
  checkBitmap(spaceship_png, "spaceship.png");

  //sounds
  ALLEGRO_SAMPLE* bip_wav = al_load_sample("./sounds/bip.wav");
  ALLEGRO_SAMPLE* bonus_wav = al_load_sample("./sounds/bonus.wav");
  ALLEGRO_SAMPLE* button_wav = al_load_sample("./sounds/button.wav");
  ALLEGRO_SAMPLE* fall_wav = al_load_sample("./sounds/fall.wav");
  ALLEGRO_SAMPLE* lose_wav = al_load_sample("./sounds/lose.wav");
  ALLEGRO_SAMPLE* Street_Fighter_wav = al_load_sample("./sounds/Street_Fighter.wav");
  ALLEGRO_SAMPLE* win_wav = al_load_sample("./sounds/win.wav");
  ALLEGRO_SAMPLE* menu_wav = al_load_sample("./sounds/menu.wav");
  ALLEGRO_SAMPLE* finish_wav = al_load_sample("./sounds/finish.wav");

  // check if sounds are loaded correctly
  checkSample(bip_wav, "bip.wav");
  checkSample(bonus_wav, "bonus.wav");
  checkSample(button_wav, "button.wav");
  checkSample(fall_wav, "fall.wav");
  checkSample(lose_wav, "lose.wav");
  checkSample(Street_Fighter_wav, "Street_Fighter.wav");
  checkSample(win_wav, "win.wav");
  checkSample(menu_wav, "menu.wav");
  checkSample(finish_wav, "finish.wav");

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

  ALLEGRO_EVENT event;

  Point score_pos({score_x, score_y});
  Point highscore_pos({highscore_x, highscore_y});

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
  
  int index, game_score = 0;  
  bool finish_all_lvl = false;
  bool showMenu = true;
  vector<string> game_levels = executeCommand("./research_jsonFile.sh ./data level_");

  if (game_levels.empty()) {
    std::cerr << "Erreur : aucun niveau trouvé par research_jsonFile.sh\n";
    return -1;
  }

  Games game = Games(game_levels.at(index), game_score);
  Point temp_direction = game.ball.getDirection();

  while (true) {
    bool done = false; 
    bool restartGame = false;

    if (showMenu) {
      menu_button(start_png, event, queue, button_wav, menu_wav, done, button_exit, button_play);
      if (done) break;
      showMenu = false;
    }
    
    if (index < game_levels.size()) {
      forceChangeLevel(index, game_score, game, temp_direction, game_levels);
    } else { done = true; restartGame = false; finish_all_lvl = true; }
    

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
          else if (event.keyboard.keycode == ALLEGRO_KEY_0) { index = 0, forceChangeLevel(0, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_1) { index = 1, forceChangeLevel(1, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_2) { index = 2, forceChangeLevel(2, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_3) { index = 3, forceChangeLevel(3, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_4) { index = 4, forceChangeLevel(4, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_5) { index = 5, forceChangeLevel(5, game_score, game, temp_direction, game_levels); }
          else if (event.keyboard.keycode == ALLEGRO_KEY_6) { index = 6, forceChangeLevel(6, game_score, game, temp_direction, game_levels); }
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
          if (check_direction_changed(game.ball.getDirection(), temp_direction)) {
              al_play_sample(bip_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          }

          if (game.ball.isFalling()) {
            game.spaceship.damage();
            game.ball.reset();
            game.ball.setFalling(false);
            if (game.spaceship.getHealth() > 0) {
              al_play_sample(fall_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
          } else {
            if (game.ball.inMouvement()) {
              game.checkCollisions();
              game.ball.move(game.spaceship.getPosition(),
                             game.spaceship.getWidth(),
                             game.spaceship.getHeight());
            } else {
              game.ball.move(game.spaceship.getPosition(), 
                             game.spaceship.getHeight());
            }
          }

          if (game.lose()) {
            al_stop_sample(&sound_game_id);
            al_play_sample(lose_wav, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            cout << "\nGame Over... Les briques ont gagné cette fois-ci. 🧱" << endl;
            cout << "Score atteint : " << game.getScore() << endl;
            cout << "Highscore : " << game.getHighScore() << "\n" << endl;
            menu_button(lose_png, event, queue, button_wav, menu_wav, restartGame, button_yes, button_no);
            game_score, index = 0;
            done = true;
          } else if (game.win()) {
            al_stop_sample(&sound_game_id);
            int score = game.getScore();
            cout << "\nFélicitations ! Tu as brisé toutes les briques ! 🎇" << endl;
            cout << "Score atteint : " << game.getScore() << endl;
            cout << "Highscore : " << game.getHighScore() << "\n" << endl;
            menu(win_png, event, queue, win_wav, font, "Press any key to continue...");
            index++;
            restartGame = true;
            game_score = score;
            done = restartGame;
          } else {
            al_clear_to_color(WHITE);
            al_draw_bitmap(background_png, 0, 0, 0);
            ALLEGRO_BITMAP* heart_png = choose_heartFile(heart_1_png, heart_2_png, heart_3_png, game);
            display_info_game(score_png, highScore_png, heart_png, font, game, score_pos, highscore_pos);
            game.draw();
            display_spaceship(spaceship_png, game);
            al_flip_display();
          }
          break;
        }
        default:
          break;
      }
    }
    game.saveHighScore();
    al_stop_sample(&sound_game_id);
    if (!restartGame) { break; }
  }
  if (finish_all_lvl) {
    cout << "Vous avez finis ARKANOID !!!";
    cout << "Score atteint : " << game.getScore() << endl;
    cout << "Highscore : " << game.getHighScore() << "\n" << endl;
    menu(finish_png, event, queue, finish_wav, font, "Press any key to exit...");
  }

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
  al_destroy_bitmap(finish_png);
  al_destroy_bitmap(heart_1_png);
  al_destroy_bitmap(heart_2_png);
  al_destroy_bitmap(heart_3_png);
  al_destroy_bitmap(highScore_png);
  al_destroy_bitmap(score_png);
  // sounds
  al_destroy_sample(win_wav);
  al_destroy_sample(fall_wav);
  al_destroy_sample(button_wav);
  al_destroy_sample(Street_Fighter_wav);
  al_destroy_sample(bonus_wav);
  al_destroy_sample(bip_wav);
  al_destroy_sample(menu_wav);
  al_destroy_sample(finish_wav);
  al_destroy_sample(lose_wav);
  al_uninstall_audio();

  return 0;
}
