#pragma once

#include <memory>
#include "f_state.hpp"

namespace Sample
{
struct Game
{
  void create(Fatty::ThreeDState&);
  void destroy(Fatty::ThreeDState&);
  void pause(Fatty::ThreeDState&);
  void resume(Fatty::ThreeDState&);
  void keyPressed(Fatty::ThreeDState&, const sf::Event::KeyEvent&);
  void keyReleased(Fatty::ThreeDState&, const sf::Event::KeyEvent&);
  void tick(Fatty::ThreeDState&);
  void draw(Fatty::ThreeDState&);
  Game();
  ~Game();

 private:
  struct Impl;
  std::unique_ptr<Impl> m_impl;
};

}  // namespace Sample