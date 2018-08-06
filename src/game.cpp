#include "game.hpp"

namespace Fatty
{
struct Game::Impl
{
  size_t r = 0;
  size_t g = 0;
  size_t b = 0;
};

Game::Game(sf::RenderWindow& window) : m_window(window), m_impl(new Impl) {}

Game::~Game() = default;

void Game::destroy() {}

void Game::pause() {}

void Game::resume() {}

void Game::keypressed(const sf::Event::KeyEvent& key) {}

void Game::keyreleased(const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      m_impl->r = (m_impl->r + 10) % 255;
      break;
    case sf::Keyboard::Right:
      m_impl->g = (m_impl->g + 10) % 255;
      break;
    default:
      break;
  }
}

void Game::draw()
{
  m_window.clear(sf::Color(m_impl->r, m_impl->g, m_impl->b));
}

}  // namespace Fatty