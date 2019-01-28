#include <SFML/Graphics.hpp>

#include "f_state.hpp"

namespace Fatty
{
/*
Game is any class with member methods:
    void create(Fatty::State&);
    void destroy(Fatty::State&);
    void pause(Fatty::State&);
    void resume(Fatty::State&);
    void key_pressed(Fatty::State&, const sf::Event::KeyEvent&);
    void key_released(Fatty::State&, const sf::Event::KeyEvent&);
    void tick(Fatty::State&);
    void draw(Fatty::State&);
*/

template <typename Game>
int Main(Game& game, int argc, char* argv[])
{
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window(sf::VideoMode(800, 600), "sfml_experiments",
                          sf::Style::Default, settings);

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
}  // namespace Fatty