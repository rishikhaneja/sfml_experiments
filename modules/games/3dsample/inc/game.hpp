#pragma once

#include "f_state.hpp"

namespace Sample
{
struct Game
{
  void create(Fatty::ThreeDState&);
  void destroy(Fatty::ThreeDState&);
  void pause(Fatty::ThreeDState&);
  void resume(Fatty::ThreeDState&);
  void key_pressed(Fatty::ThreeDState&, const sf::Event::KeyEvent&);
  void key_released(Fatty::ThreeDState&, const sf::Event::KeyEvent&);
  void tick(Fatty::ThreeDState&);
  void draw(Fatty::ThreeDState&);
};

}  // namespace Sample