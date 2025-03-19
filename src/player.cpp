#include "player.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

Player::Player(float X, float Y, float width, float height) : X(X), Y(Y), width(width), height(height)
{
  setSize(sf::Vector2f(width, height));
  setPosition(X, Y);
  setFillColor(sf::Color::Green);
}

sf::Vector2f Player::getCenter()
{
  return sf::Vector2f(X + width / 2, Y + height / 2);
}

void Player::setSpeed(float newSpeed)
{
  speed = newSpeed;
}

float Player::getSpeed()
{
  return speed;
}
