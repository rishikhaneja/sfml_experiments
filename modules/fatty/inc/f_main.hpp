#include <SFML/Graphics.hpp>

#include "f_state.hpp"

namespace Fatty
{
/*
Game is any class with member methods:
    void create(Fatty::TwoDState&);
    void destroy(Fatty::TwoDState&);
    void pause(Fatty::TwoDState&);
    void resume(Fatty::TwoDState&);
    void key_pressed(Fatty::TwoDState&, const sf::Event::KeyEvent&);
    void key_released(Fatty::TwoDState&, const sf::Event::KeyEvent&);
    void tick(Fatty::TwoDState&);
    void draw(Fatty::TwoDState&);
*/

template <typename Game, typename Window, typename State>
int Main(Game& game, const char* name)
{
  sf::ContextSettings settings;
  settings.depthBits         = 24;
  settings.stencilBits       = 8;
  settings.antialiasingLevel = 2;  // Optional
  settings.majorVersion      = 3;
  settings.minorVersion      = 2;
  settings.attributeFlags    = sf::ContextSettings::Core;

  Window window(sf::VideoMode(800, 600), name, sf::Style::Default, settings);
  window.setActive(true);

  State state{window};

  game.create(state);

  while (window.isOpen())
  {
    sf::Event event;

    while (window.pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed:
          game.destroy(state);
          window.close();
          break;

        case sf::Event::KeyPressed:
          game.key_pressed(state, event.key);
          break;

        case sf::Event::KeyReleased:
          game.key_released(state, event.key);
          break;

        case sf::Event::LostFocus:
          game.pause(state);
          break;

        case sf::Event::GainedFocus:
          game.resume(state);
          break;

        default:
          break;
      }
    }

    game.tick(state);

    game.draw(state);

    window.display();
  }
  return 0;
}

template <typename Game>
int TwoDMain(Game& game, const char* name)
{
  return Main<Game, sf::RenderWindow, TwoDState>(game, name);
}

template <typename Game>
int ThreeDMain(Game& game, const char* name)
{
  return Main<Game, sf::Window, ThreeDState>(game, name);
}

}  // namespace Fatty