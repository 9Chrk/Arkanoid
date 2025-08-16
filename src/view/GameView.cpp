using namespace std;
#include "view/GameView.hpp"


/// @brief Construct the view and initialize all Allegro subsystems and resources.
GameView::GameView(const shared_ptr<GameModel>& model)
  : model(model),
    uiConfig(make_shared<UIConfig>()),
    allegroConfig(nullptr)
{
  // Load core modules
  must_init(al_init(),                                  "allegro");
  must_init(al_install_keyboard(),                      "keyboard");
  must_init(al_install_mouse(),                         "mouse");
  must_init(al_install_audio(),                         "audio");
  must_init(al_init_acodec_addon(),                     "acodec");
  must_init(al_reserve_samples(AUDIO_RESERVED_SAMPLES), "reserve samples");

  // Enable useful add-ons
  must_init(al_init_image_addon(),      "image");
  must_init(al_init_font_addon(),       "font");
  must_init(al_init_ttf_addon(),        "ttf");
  must_init(al_init_primitives_addon(), "primitives");

  // Load bitmaps, fonts, sounds and event queue
  allegroConfig = make_shared<AllegroConfig>();

  // Configure the display before creating bitmaps
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, ANTIALIASING_SAMPLES, ALLEGRO_SUGGEST);
  al_set_new_display_flags(ALLEGRO_WINDOWED);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_CONVERT_BITMAP);

  // Start the main timer used to refresh the game
  al_start_timer(allegroConfig->timer);
}

// Getters

[[gnu::pure]] shared_ptr<AllegroConfig> GameView::getAllegroConfig() const {
  return allegroConfig;
}

[[gnu::pure]] shared_ptr<UIConfig> GameView::getUIConfig() const {
  return uiConfig;
}

// Sounds methods

// Play a sound sample.  When loop is true the sound is played repeatedly
// until stopSound is called.
void GameView::playSound(ALLEGRO_SAMPLE* sound, ALLEGRO_SAMPLE_ID* id, bool loop) const {
  ALLEGRO_PLAYMODE mode = loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE;
  al_play_sample(sound, 1.0, 0.0, 1.0, mode, id);
}

// Stop a previously started sound.
void GameView::stopSound(ALLEGRO_SAMPLE_ID* id) const {
  al_stop_sample(id);
}

// Draw methods

// Draw a simple filled circle at a position with a given radius and color.
void GameView::drawCircle(const Point& position, float radius, ALLEGRO_COLOR color) const {
  al_draw_filled_circle(position.x, position.y, radius, color);
}

// Draw a filled rectangle with an optional frame around it.
void GameView::drawRectangle(const Point& upLeft, const Point& downRight, ALLEGRO_COLOR fillColor, ALLEGRO_COLOR frameColor) const {
  al_draw_filled_rectangle(upLeft.x, upLeft.y, downRight.x, downRight.y, fillColor);
  al_draw_rectangle(upLeft.x, upLeft.y, downRight.x, downRight.y, frameColor, FRAME_THICKNESS);
}

// Render the ball based on its position and radius from the model.
void GameView::drawBall() const {
  const Ball& ball = model->getBall();
  Point position = ball.getPosition();
  drawCircle(position, ball.getRadius(), WHITE);
}

// Render a single brick and its bonus abbreviation when needed.
void GameView::drawBrick(const Brick& brick) const {
  ALLEGRO_COLOR fillColor = getColor(uiConfig->settings["brick_colors"][to_string(brick.getScore())].get<string>());
  ALLEGRO_COLOR frameColor =  (brick.getScore() == SECOND_LIFE && !brick.getSecondLife()) ? RED : BLACK;

  auto [upLeft, downRight] = brick.diag_coor();
  drawRectangle(upLeft, downRight, fillColor, frameColor);
  drawBonusAbbreviation(brick);
}

// Draw every non destroyed brick on the board.
void GameView::drawBricks() const {
  const vector<Brick>& bricks = model->getBricks();
  for (const auto& brick : bricks) {
    if (!brick.isDestroyed()) drawBrick(brick);
  }
}

// Draw the abbreviation of the bonus contained in a brick.
void GameView::drawBonusAbbreviation(const Brick& brick) const {
  Point position = brick.getPosition();
  const BonusType& bonus = brick.getBonus();

  if (bonus != BonusType::NONE) {
    string txt = Bonus::getAbbreviation(bonus);
    float textWidth  = static_cast<float>(al_get_text_width(allegroConfig->fontBonus, txt.c_str()));
    float textHeight = static_cast<float>(al_get_font_line_height(allegroConfig->fontBonus));

    float drawX = position.x - textWidth/2;
    float drawY = position.y - textHeight/2;
    drawY -= 2.0f; // Correction of 2 pixels upwards due to borders

    al_draw_text(allegroConfig->fontBonus, BLACK, drawX, drawY, 0, txt.c_str());
  }
}

// Render the shape of a bonus.
void GameView::drawBonus(const Bonus& bonus) const {
  ALLEGRO_COLOR color = getColor(uiConfig->settings["bonus_colors"][Bonus::getAbbreviation(bonus.getType())].get<string>());

  float w = bonus.getWidth();
  float h = bonus.getHeight();
  Point position = bonus.getPosition();

  const Point& leftArc     = {position.x - w/2, position.y};
  const Point& rightArc    = {position.x + w/2, position.y};
  auto [upLeft, downRight] = bonus.diag_coor();

  drawCircle(leftArc,  h/2, color);
  drawCircle(rightArc, h/2, color);
  drawRectangle(upLeft, downRight, color, BLACK);
}

// Draw each active bonus falling on the playfield.
void GameView::drawBonuses() const {
  const vector<shared_ptr<Bonus>>& bonuses = model->getBonuses();
  for (const auto& bonus : bonuses) {
    if (bonus->isActive()) drawBonus(*bonus);
  }
}

// Display all game elements

// Render the whole frame: background, HUD and all moving elements.
void GameView::render() const {
  al_clear_to_color(WHITE);
  al_draw_bitmap(allegroConfig->background_png, 0, 0, 0);
  displayHUD();
  drawBall();    
  drawBricks();
  drawBonuses();
  displaySpaceship();
  al_flip_display();
}

// Display methods

// Show a full screen image (used for menus and transitions).
void GameView::displayImage(ALLEGRO_BITMAP* bmp) const {
  if (bmp) {
    al_clear_to_color(WHITE);
    al_draw_bitmap(bmp, 0, 0, 0);
    al_flip_display();
  }
}

// Draw the score and high score on the screen.
void GameView::displayScore() const {
  al_draw_bitmap(allegroConfig->score_png, 0, 0, 0);
  al_draw_bitmap(allegroConfig->highScore_png, 0, 0, 0);
  al_draw_text(allegroConfig->font, WHITE, uiConfig->scorePos.x, uiConfig->scorePos.y, ALLEGRO_ALIGN_CENTER, to_string(model->getScore()).c_str());
  al_draw_text(allegroConfig->font, WHITE, uiConfig->highscorePos.x, uiConfig->highscorePos.y, ALLEGRO_ALIGN_CENTER, to_string(model->getHighScore()).c_str());
}

// Display the HUD including current lives.
void GameView::displayHUD() const {
  displayScore();
  al_draw_bitmap(chooseHeartFile(), 0, 0, 0);
}

// Render the player's spaceship scaled to the model size.
void GameView::displaySpaceship() const {
  const Spaceship& spaceship = model->getSpaceship();
  Point spaceshipPos = spaceship.getPosition();
  float spaceshipWidth = spaceship.getWidth();
  
  float imgW = static_cast<float>(al_get_bitmap_width(allegroConfig->spaceship_png));
  float imgH = static_cast<float>(al_get_bitmap_height(allegroConfig->spaceship_png));
  float spaceshipHeight = imgH;
  
  al_draw_scaled_bitmap(allegroConfig->spaceship_png, 0, 0, imgW, imgH,
    spaceshipPos.x - spaceshipWidth/2, spaceshipPos.y - spaceshipHeight/2, spaceshipWidth, spaceshipHeight, 0);
}

// Helper methods

// Select the heart image according to remaining lives.
[[gnu::pure]] ALLEGRO_BITMAP* GameView::chooseHeartFile() const {
  int lives = model->getSpaceship().getHealth();
  if      (lives == 1) { return allegroConfig->heart_1_png; }
  else if (lives == 2) { return allegroConfig->heart_2_png; }
  else { return allegroConfig->heart_3_png; }
}

// Highlight the button currently hovered with the mouse.
void GameView::handleButtonHover(ALLEGRO_BITMAP* image_png, bool hoverPlay, bool hoverExit,
                                const Rectangle& buttonON, const Rectangle& buttonOFF)
{
  displayImage(image_png);
  
  if (hoverPlay || hoverExit) {
    pair<Point, Point> temp = hoverPlay? buttonON.diag_coor() : buttonOFF.diag_coor();
    playSound(allegroConfig->button_wav);
    al_draw_rectangle(temp.first.x, temp.first.y, temp.second.x, temp.second.y, WHITE, FRAME_THICKNESS);
  }
  al_flip_display();
}

// Handle mouse events on the menu: hovering and clicking buttons.
bool GameView::processMenuMouseEvent(const ALLEGRO_EVENT& event, ALLEGRO_BITMAP* image_png,
                                     bool& hoverPlay, bool& hoverExit, bool& done,
                                     const Rectangle& buttonON, const Rectangle& buttonOFF)
{
  if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
    Point mousePos = {static_cast<float>(event.mouse.x), static_cast<float>(event.mouse.y)};
    bool newHoverPlay = buttonON.contains(mousePos);
    bool newHoverExit = buttonOFF.contains(mousePos);
    
    if (newHoverPlay != hoverPlay || newHoverExit != hoverExit) {
      hoverPlay = newHoverPlay;
      hoverExit = newHoverExit;
      handleButtonHover(image_png, hoverPlay, hoverExit, buttonON, buttonOFF);
    }
  }
  else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    Point clickPos = {static_cast<float>(event.mouse.x), static_cast<float>(event.mouse.y)};
    if (buttonON.contains(clickPos) || buttonOFF.contains(clickPos)) {
      done = buttonON.contains(clickPos);
      return true;
    }
  }
  return false;
}

// Menu methods

// Display a text based menu and wait for the player to press a key.
void GameView::menu(ALLEGRO_BITMAP* image_png, const string& text, ALLEGRO_SAMPLE* sound, bool soundLoop, bool showScore) {
  ALLEGRO_EVENT event = allegroConfig->event;
  ALLEGRO_SAMPLE_ID soundMenuID;
  bool pressAnyButton = false;

  displayImage(image_png);
  if (showScore) { displayScore(); }

  al_flip_display();
  playSound(sound, &soundMenuID, soundLoop);
  al_rest(PAUSE_SECONDS);

  float fontHeight = static_cast<float>(al_get_font_line_height(allegroConfig->font));
  al_draw_text(allegroConfig->font, WHITE, GAME_WIDTH * PLAYABLE_RATIO, GAME_HEIGHT - fontHeight, ALLEGRO_ALIGN_CENTER, text.c_str());
  al_flip_display();

  while (!pressAnyButton) { 
    al_flush_event_queue(allegroConfig->queue);
    al_wait_for_event(allegroConfig->queue, &event);
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) { pressAnyButton = true; }
  }
  al_stop_sample(&soundMenuID);
}

// Display a menu with Play/Exit buttons and handle the user's selection.
void GameView::menuButton(ALLEGRO_BITMAP* image_png, bool &done, const Rectangle& buttonON, const Rectangle& buttonOFF) {
  ALLEGRO_EVENT event = allegroConfig->event;

  bool clickOnButton = false;
  bool hoverPlay = false;
  bool hoverExit = false;
  
  displayImage(image_png);
  al_flip_display();
  ALLEGRO_SAMPLE_ID soundMenuID;
  playSound(allegroConfig->menu_wav, &soundMenuID, true);
  
  while (!clickOnButton) {
    al_flush_event_queue(allegroConfig->queue);
    al_wait_for_event(allegroConfig->queue, &event);
    
    clickOnButton = processMenuMouseEvent(event, image_png, hoverPlay, hoverExit, done, buttonON, buttonOFF);
  }
  al_stop_sample(&soundMenuID);
}
