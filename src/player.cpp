#include "player.h"
#include <SFML/System/Vector2.hpp>
#include <array>

Player::Player(float X, float Y, float width, float height) : X(X), Y(Y), width(width), height(height)
{
  rectangle.setSize(sf::Vector2f(width, height));  // Corrected function name
  rectangle.setPosition(X, Y);                     // Set initial position
}

sf::Vector2f Player::getCenter()
{
  return sf::Vector2f(X + width / 2, Y + height / 2);
}

sf::Vector2f Player::getPosition()
{
  return sf::Vector2f(X, Y);
}

/*std::array<sf::RectangleShape, 4> Player::getSurroundWalls() {}*/
