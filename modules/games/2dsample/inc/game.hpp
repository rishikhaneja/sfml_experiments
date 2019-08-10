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
  void keyPressed(Fatty::TwoDState&, const sf::Event::KeyEvent&);
  void keyReleased(Fatty::TwoDState&, const sf::Event::KeyEvent&);
  void tick(Fatty::TwoDState&);
  void draw(Fatty::TwoDState&);
};

}  // namespace Sample