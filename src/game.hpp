#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Fatty
{
struct Game
{
  Game(sf::RenderWindow&);
  ~Game();
  void destroy();
  void pause();
  void resume();
  void keypressed(const sf::Event::KeyEvent&);
  void keyreleased(const sf::Event::KeyEvent&);
  void draw();

 private:
  sf::RenderWindow& m_window;
  struct Impl;
  std::unique_ptr<Impl> m_impl;
};  // struct Game
}  // namespace Fatty