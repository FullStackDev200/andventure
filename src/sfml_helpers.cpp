#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include "room.h"

using namespace std;

namespace sfml_helpers
{

  bool isContained(const sf::RectangleShape &player, const sf::RectangleShape &room)
  {
    // Get the global bounds of both rectangles
    const sf::FloatRect playerBounds = player.getGlobalBounds();
    const sf::FloatRect roomBounds = room.getGlobalBounds();

    // Check if all corners of the player are within the room
    return playerBounds.left >= roomBounds.left && playerBounds.top >= roomBounds.top && playerBounds.left + playerBounds.width <= roomBounds.left + roomBounds.width &&
           playerBounds.top + playerBounds.height <= roomBounds.top + roomBounds.height;
  }

  sf::RectangleShape getThickLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color lineColor, float thickness)
  {
    // Calculate the direction and length of the line
    sf::Vector2f direction = point2 - point1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize the direction vector
    direction /= length;

    // Create a rectangle shape to represent the line with the desired thickness
    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setFillColor(lineColor);

    // Set the orientation of the rectangle to match the line's direction
    line.setPosition(point1);
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;  // Convert to degrees
    line.setRotation(angle);

    // Draw the line (rectangle)
    return line;
  }

  bool isOnWalkableArea(sf::RectangleShape player, vector<Room> shapes)
  {
    sf::FloatRect playerBounds = player.getGlobalBounds();

    bool isWalkable = false;
    for (const auto &shape : shapes)
    {
      if (isContained(player, shape))
      {
        isWalkable = true;
        break;
      }
    }
    return isWalkable;
  }

  sf::RectangleShape getRectagleWith2Vectors(const sf::Vector2f &point1, const sf::Vector2f &point2, float wallWidth)
  {
    sf::RectangleShape newRectangle;
    newRectangle.setSize(sf::Vector2f(max(abs(point1.x - point2.x), wallWidth), max(abs(point1.y - point2.y), wallWidth)));
    newRectangle.setPosition(sf::Vector2f(min(point1.x, point2.x), min(point1.y, point2.y)));
    return newRectangle;
  }
}  // namespace sfml_helpers
