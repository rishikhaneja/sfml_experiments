#pragma once

#include <SFML/Graphics.hpp>

namespace Fatty
{
template <typename Window>
struct State
{
  Window& window;
};

using TwoDState   = State<sf::RenderWindow>;
using ThreeDState = State<sf::Window>;

}  // namespace Fatty