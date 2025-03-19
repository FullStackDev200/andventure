#include "room.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

Room::Room(float X, float Y, float width, float height) : X(X), Y(Y), width(width), height(height)
{
  setSize(sf::Vector2f(width, height));
  setPosition(sf::Vector2f(X, Y));
  setFillColor(sf::Color::Green);
}

std::array<sf::Vector2f, 4> Room::getEdgePoints(int wallWidth)
{
  std::array<sf::Vector2f, 4> walls;
  sf::Vector2f wall1 = sf::Vector2f(getPosition().x - wallWidth, getPosition().y - wallWidth);
  sf::Vector2f wall2 = sf::Vector2f(getPosition().x + wallWidth, getPosition().y + getSize().y + wallWidth);
  sf::Vector2f wall3 = sf::Vector2f(getPosition().x + getSize().x + wallWidth, getPosition().y + getSize().y + wallWidth);
  sf::Vector2f wall4 = sf::Vector2f(getPosition().x + getSize().x + wallWidth, getPosition().y - wallWidth);

  walls[0] = wall1;
  walls[1] = wall2;
  walls[2] = wall3;
  walls[3] = wall4;

  return walls;
}
