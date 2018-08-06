#include "game.hpp"

namespace Fatty
{
struct Game::Impl
{
  bool   right_pressed = false;
  bool   left_pressed  = false;
  size_t r             = 0;
  size_t g             = 0;
  size_t b             = 0;
};

Game::Game(sf::RenderWindow& window) : m_window(window), m_impl(new Impl) {}

Game::~Game() = default;

void Game::destroy() {}

void Game::pause() {}

void Game::resume() {}

void Game::keypressed(const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      m_impl->left_pressed = true;
      break;
    case sf::Keyboard::Right:
      m_impl->right_pressed = true;
      break;
    default:
      break;
  }
}

void Game::keyreleased(const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      m_impl->left_pressed = false;
      break;
    case sf::Keyboard::Right:
      m_impl->right_pressed = false;
      break;
    default:
      break;
  }
}

void Game::tick()
{
  if (m_impl->left_pressed)
  {
    m_impl->r = (m_impl->r + 1) % 255;
  }
  if (m_impl->right_pressed)
  {
    m_impl->g = (m_impl->g + 1) % 255;
  }
}

void Game::draw()
{
  m_window.clear(sf::Color(m_impl->r, m_impl->g, m_impl->b));
}

}  // namespace Fatty