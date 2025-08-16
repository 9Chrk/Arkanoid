using namespace std;
#include "view/AllegroInputAdapter.hpp"


optional<InputAction> AllegroInputAdapter::translate(const ALLEGRO_EVENT& event) const {
  switch (event.type) {
    case ALLEGRO_EVENT_KEY_DOWN: {
      switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_A:
        case ALLEGRO_KEY_Q:
         return InputAction{InputActionType::MoveLeft};  // move paddle to the left

        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_D:
        case ALLEGRO_KEY_P:
          return InputAction{InputActionType::MoveRight}; // move paddle to the right

        case ALLEGRO_KEY_0: case ALLEGRO_KEY_1: case ALLEGRO_KEY_2:
        case ALLEGRO_KEY_3: case ALLEGRO_KEY_4: case ALLEGRO_KEY_5:
        case ALLEGRO_KEY_6: case ALLEGRO_KEY_7: case ALLEGRO_KEY_8: {
          int index = event.keyboard.keycode - ALLEGRO_KEY_0;
          return InputAction{InputActionType::LevelChange, index};
        }
        case ALLEGRO_KEY_R:      return InputAction{InputActionType::Reset};
        case ALLEGRO_KEY_SPACE:  return InputAction{InputActionType::Launch};
        case ALLEGRO_KEY_ESCAPE: return InputAction{InputActionType::Quit};
        default: return nullopt;
      }
    }
    case ALLEGRO_EVENT_TIMER:         return InputAction{InputActionType::TimerTick};
    case ALLEGRO_EVENT_MOUSE_AXES:    return InputAction{InputActionType::MouseMove, event.mouse.x, event.mouse.y};
    case ALLEGRO_EVENT_DISPLAY_CLOSE: return InputAction{InputActionType::Quit};
    default: return nullopt;
  }
}
