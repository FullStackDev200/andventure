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
#include <utility>
#include "room.h"

using namespace std;

namespace sfml_helpers
{

  auto get_formula(pair<float, float> &middle1, pair<float, float> &middle2)
  {
    return [middle1, middle2](int x) { return (middle2.first - middle1.first) / (middle2.second - middle1.second) * (x - middle1.second); };
  }

  bool isContained(const sf::RectangleShape &player, const sf::RectangleShape &room)
  {
    // Get the global bounds of both rectangles
    const sf::FloatRect playerBounds = player.getGlobalBounds();
    const sf::FloatRect roomBounds = room.getGlobalBounds();

    // Check if all corners of the player are within the room
    return playerBounds.left >= roomBounds.left && playerBounds.top >= roomBounds.top && playerBounds.left + playerBounds.width <= roomBounds.left + roomBounds.width &&
           playerBounds.top + playerBounds.height <= roomBounds.top + roomBounds.height;
  }

  sf::RectangleShape getThickLine(sf::RenderWindow &window, sf::Vector2f point1, sf::Vector2f point2, sf::Color lineColor, float thickness)
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
}  // namespace sfml_helpers
