using namespace std;
#include "AllegroConfig.hpp"


AllegroConfig::AllegroConfig() : event{} {
  try {
    // components
    
    queue     = al_create_event_queue();
    timer     = al_create_timer(1.0/refreshPerSecond);
    display   = al_create_display(GameWidth, GameHeight);
    font.reset(al_load_font("./assets/fonts/Distant_galaxy.ttf", 20, 0));
    fontBonus.reset(al_load_font("./assets/fonts/Pixeled.ttf", 6, 0));

    // check components
    must_init(display,   "display");
    must_init(timer,     "timer");
    must_init(font.get(),      "font");
    must_init(fontBonus.get(), "fontBonus");
    must_init(queue,     "event queue");

    // images
    start_png.reset(loadBitmap("./assets/images/start.png"));
    background_png.reset(loadBitmap("./assets/images/background.png"));
    lose_png.reset(loadBitmap("./assets/images/lose.png"));
    win_png.reset(loadBitmap("./assets/images/win.png"));
    spaceship_png.reset(loadBitmap("./assets/images/spaceship.png"));
    heart_1_png.reset(loadBitmap("./assets/images/1heart.png"));
    heart_2_png.reset(loadBitmap("./assets/images/2heart.png"));
    heart_3_png.reset(loadBitmap("./assets/images/3heart.png"));
    score_png.reset(loadBitmap("./assets/images/score.png"));
    highScore_png.reset(loadBitmap("./assets/images/high_score.png"));
    finish_png.reset(loadBitmap("./assets/images/finish.png"));
    
    // sounds
    bip_wav.reset(loadSample("./assets/sounds/bip.wav"));
    bonus_wav.reset(loadSample("./assets/sounds/bonus.wav"));
    button_wav.reset(loadSample("./assets/sounds/button.wav"));
    fall_wav.reset(loadSample("./assets/sounds/fall.wav"));
    lose_wav.reset(loadSample("./assets/sounds/lose.wav"));
    Street_Fighter_wav.reset(loadSample("./assets/sounds/Street_Fighter.wav"));
    win_wav.reset(loadSample("./assets/sounds/win.wav"));
    menu_wav.reset(loadSample("./assets/sounds/menu.wav"));
    finish_wav.reset(loadSample("./assets/sounds/finish.wav"));
    
    // Register event sources
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
  }
  catch (const std::exception& e) {
    std::cerr << "Fatal asset load error: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

AllegroConfig::~AllegroConfig() {
  // components
  font.reset();
  fontBonus.reset();
  start_png.reset();
  background_png.reset();
  lose_png.reset();
  win_png.reset();
  spaceship_png.reset();
  heart_1_png.reset();
  heart_2_png.reset();
  heart_3_png.reset();
  score_png.reset();
  highScore_png.reset();
  finish_png.reset();
  bip_wav.reset();
  bonus_wav.reset();
  button_wav.reset();
  fall_wav.reset();
  lose_wav.reset();
  Street_Fighter_wav.reset();
  win_wav.reset();
  menu_wav.reset();
  finish_wav.reset();

  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  al_uninstall_audio();
}
