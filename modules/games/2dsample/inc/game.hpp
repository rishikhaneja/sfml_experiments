#pragma once

#include "f_state.hpp"

namespace Sample
{
struct Game
{
  void create(Fatty::TwoDState&);
  void destroy(Fatty::TwoDState&);
  void pause(Fatty::TwoDState&);
  void resume(Fatty::TwoDState&);
  void key_pressed(Fatty::TwoDState&, const sf::Event::KeyEvent&);
  void key_released(Fatty::TwoDState&, const sf::Event::KeyEvent&);
  void tick(Fatty::TwoDState&);
  void draw(Fatty::TwoDState&);
};

}  // namespace Sample