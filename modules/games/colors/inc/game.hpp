#pragma once

#include "f_state.hpp"

namespace Colors
{
struct Game
{
  void create(Fatty::State&);
  void destroy(Fatty::State&);
  void pause(Fatty::State&);
  void resume(Fatty::State&);
  void key_pressed(Fatty::State&, const sf::Event::KeyEvent&);
  void key_released(Fatty::State&, const sf::Event::KeyEvent&);
  void tick(Fatty::State&);
  void draw(Fatty::State&);
};

}  // namespace Colors