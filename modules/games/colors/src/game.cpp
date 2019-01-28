#include "game.hpp"

namespace Colors
{
bool   right_pressed = false;
bool   left_pressed  = false;
size_t r             = 0;
size_t g             = 0;
size_t b             = 0;

void Game::create(Fatty::State&) {}

void Game::destroy(Fatty::State&) {}

void Game::pause(Fatty::State&) {}

void Game::resume(Fatty::State&) {}

void Game::key_pressed(Fatty::State&, const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      left_pressed = true;
      break;
    case sf::Keyboard::Right:
      right_pressed = true;
      break;
    default:
      break;
  }
}

void Game::key_released(Fatty::State&, const sf::Event::KeyEvent& key)
{
  switch (key.code)
  {
    case sf::Keyboard::Left:
      left_pressed = false;
      break;
    case sf::Keyboard::Right:
      right_pressed = false;
      break;
    default:
      break;
  }
}

void Game::tick(Fatty::State&)
{
  if (left_pressed)
  {
    r = (r + 1) % 255;
  }
  if (right_pressed)
  {
    g = (g + 1) % 255;
  }
}

void Game::draw(Fatty::State& state) { state.window.clear(sf::Color(r, g, b)); }

}  // namespace Colors