#pragma once
#include "core/utils.hpp"
#include "view/utils.hpp"
#include "input/InputAction.hpp"


class AllegroInputAdapter {
 public:
  std::optional<InputAction> translate(const ALLEGRO_EVENT& event) const;
};
