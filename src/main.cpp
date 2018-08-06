#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.hpp"

int main(int argc, char *argv[])
{
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::RenderWindow window(sf::VideoMode(800, 600), "sfml_experiments",
                          sf::Style::Default, settings);
  Fatty::Game      game(window);

  while (window.isOpen())
  {
    sf::Event event;

    while (window.pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed:
          game.destroy();
          window.close();
          break;

        case sf::Event::KeyPressed:
          game.keypressed(event.key);
          break;

        case sf::Event::KeyReleased:
          game.keyreleased(event.key);
          break;

        case sf::Event::LostFocus:
          game.pause();
          break;

        case sf::Event::GainedFocus:
          game.resume();
          break;

        default:
          break;
      }
    }

    game.draw();

    window.display();
  }
}