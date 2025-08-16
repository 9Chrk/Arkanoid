using namespace std;
#include "view/AllegroConfig.hpp"


AllegroConfig::AllegroConfig() : event{} {
  try {
    // components
    queue     = al_create_event_queue();
    timer     = al_create_timer(1.0/REFRESH_PER_SECOND);
    display   = al_create_display(GAME_WIDTH, GAME_HEIGHT);
    font      = al_load_font("./assets/fonts/Distant_galaxy.ttf", FONT_SIZE, 0);
    fontBonus = al_load_font("./assets/fonts/Pixeled.ttf", FONT_BONUS_SIZE, 0);

    // check components
    must_init(display,   "display");
    must_init(timer,     "timer");
    must_init(font,      "font");
    must_init(fontBonus, "fontBonus");
    must_init(queue,     "event queue");

    // images
    start_png      = loadBitmap("./assets/images/start.png");
    background_png = loadBitmap("./assets/images/background.png");
    lose_png       = loadBitmap("./assets/images/lose.png");
    win_png        = loadBitmap("./assets/images/win.png");
    spaceship_png  = loadBitmap("./assets/images/spaceship.png");
    heart_1_png    = loadBitmap("./assets/images/1heart.png");
    heart_2_png    = loadBitmap("./assets/images/2heart.png");
    heart_3_png    = loadBitmap("./assets/images/3heart.png");
    score_png      = loadBitmap("./assets/images/score.png");
    highScore_png  = loadBitmap("./assets/images/high_score.png");
    finish_png     = loadBitmap("./assets/images/finish.png");
    
    // sounds
    bip_wav            = loadSample("./assets/sounds/bip.wav");
    bonus_wav          = loadSample("./assets/sounds/bonus.wav");
    button_wav         = loadSample("./assets/sounds/button.wav");
    fall_wav           = loadSample("./assets/sounds/fall.wav");
    lose_wav           = loadSample("./assets/sounds/lose.wav");
    street_Fighter_wav = loadSample("./assets/sounds/street_Fighter.wav");
    win_wav            = loadSample("./assets/sounds/win.wav");
    menu_wav           = loadSample("./assets/sounds/menu.wav");
    finish_wav         = loadSample("./assets/sounds/finish.wav");
    
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
  al_stop_samples(); 
  // components
  al_destroy_font(font);
  al_destroy_font(fontBonus);
  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  // images
  al_destroy_bitmap(start_png);
  al_destroy_bitmap(background_png);
  al_destroy_bitmap(lose_png);
  al_destroy_bitmap(win_png);
  al_destroy_bitmap(spaceship_png);
  al_destroy_bitmap(heart_1_png);
  al_destroy_bitmap(heart_2_png);
  al_destroy_bitmap(heart_3_png);
  al_destroy_bitmap(score_png);
  al_destroy_bitmap(highScore_png);
  al_destroy_bitmap(finish_png);
  // sounds
  al_destroy_sample(bip_wav);
  al_destroy_sample(bonus_wav);
  al_destroy_sample(button_wav);
  al_destroy_sample(fall_wav);
  al_destroy_sample(lose_wav);
  al_destroy_sample(street_Fighter_wav);
  al_destroy_sample(win_wav);
  al_destroy_sample(menu_wav);
  al_destroy_sample(finish_wav);
  al_uninstall_audio();
}
