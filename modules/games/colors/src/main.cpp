#include <iostream>

#include "f_main.hpp"

#include "game.hpp"

int main(int argc, char *argv[])
{
  Colors::Game game;
  return Fatty::Main(game, argc, argv);
}