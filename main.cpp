#include "include/common.hpp"
#include "include/games.hpp"


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
          game.ball.inMouvement = true;
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
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
        // Verifie si collision entre la balle et les briques
        game.checkCollisions();
        // Verifie si la balle est tombé
        if (game.ball.isFalling) {
          game.spaceship.damage();
          game.ball.reset();
          game.ball.isFalling = false;
        } 
        else {
          // Met la balle en mouvement (lancement/lancée)
          if (game.ball.inMouvement) {game.ball.move(game.spaceship.getPosition(), 
                                                     game.spaceship.getWidth(),
                                                     game.spaceship.getHeight());}
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
