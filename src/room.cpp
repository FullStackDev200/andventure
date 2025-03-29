#include "room.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <ostream>

Room::Room(float X, float Y, float width, float height) : X(X), Y(Y), width(width), height(height)
{
  setSize(sf::Vector2f(width, height));
  setPosition(sf::Vector2f(X, Y));
  setFillColor(sf::Color::Green);
}

std::array<sf::Vector2f, 4> Room::getEdgePoints(int wallWidth) const
{
  std::array<sf::Vector2f, 4> walls;

  sf::Vector2f pos = getPosition();
  sf::Vector2f size = getSize();

  sf::Vector2f bottomLeft = {pos.x, pos.y};
  sf::Vector2f topLeft = {pos.x, pos.y + size.y};
  sf::Vector2f topRight = {pos.x + size.x + wallWidth, pos.y + size.y};
  sf::Vector2f bottomRight = {pos.x + size.x, pos.y};

  walls[0] = topLeft;
  walls[1] = topRight;
  walls[2] = bottomRight;
  walls[3] = bottomLeft;

  std::cout << "Top Left: " << topLeft.x << ", " << topLeft.y << "\n";
  std::cout << "Top Right: " << topRight.x << ", " << topRight.y << "\n";
  std::cout << "Bottom Right: " << bottomRight.x << ", " << bottomRight.y << "\n";
  std::cout << "Bottom Left: " << bottomLeft.x << ", " << bottomLeft.y << "\n";

  return walls;
}
