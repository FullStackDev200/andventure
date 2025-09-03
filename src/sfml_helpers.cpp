#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include "room.h"

#include "adventure_graph.hpp"

using namespace std;

namespace sfml_helpers
{

  bool isContained(const sf::RectangleShape& player, const sf::RectangleShape& room)
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
    bool isWalkable = false;
    for (const auto& shape : shapes)
    {
      if (isContained(player, shape))
      {
        isWalkable = true;
        break;
      }
    }
    return isWalkable;
  }

  sf::RectangleShape getRectagleWith2Vectors(const sf::Vector2f& point1, const sf::Vector2f& point2, float wallWidth)
  {
    sf::RectangleShape newRectangle;
    newRectangle.setSize(sf::Vector2f(max(abs(point1.x - point2.x), wallWidth), max(abs(point1.y - point2.y), wallWidth)));
    newRectangle.setPosition(sf::Vector2f(min(point1.x, point2.x), min(point1.y, point2.y)));
    return newRectangle;
  }

  std::vector<std::array<sf::Vector2f, 3>> makeNewPaths(std::vector<Room> rooms)
  {
    std::vector<std::array<sf::Vector2f, 3>> newPaths;
    std::vector<sf::Vector2f> roomsCenter(rooms.size());
    vector<std::pair<int, int>> paths = adventure_graph::get_paths();

    // if (rooms.size() < 2)
    // {                   // Add this check
    //   return newPaths;  // Return an empty vector if there are fewer than 2 rooms
    // }

    std::transform(rooms.begin(), rooms.end(), roomsCenter.begin(),
                   [](const sf::RectangleShape& room) { return sf::Vector2f(room.getPosition().x + room.getSize().x / 2, room.getPosition().y + room.getSize().y / 2); });

    for (size_t i = 0; i < paths.size(); i++)
    {
      sf::Vector2f startPoint = roomsCenter[paths[i].first];
      sf::Vector2f middlePoint = sf::Vector2f(roomsCenter[paths[i].first].x, roomsCenter[paths[i].second].y);
      sf::Vector2f endPoint = roomsCenter[paths[i].second];

      newPaths.push_back({startPoint, middlePoint, endPoint});
    }

    return newPaths;
  }

  vector<Room> vectorToRoom(const vector<pair<int, int>>& rooms, const vector<pair<int, int>>& coords, int scale)
  {
    vector<Room> myRooms;
    for (size_t i = 0; i < rooms.size(); i++)
    {
      myRooms.push_back(Room(coords[i].first * scale, coords[i].second * scale, rooms[i].first * scale, rooms[i].second * scale));
    }
    return myRooms;
  }

  sf::FloatRect getDoorChords(const Room& room, const sf::RectangleShape& path)
  {
    sf::FloatRect intersection;
    room.getGlobalBounds().intersects(path.getGlobalBounds(), intersection);
    return intersection;
  }

  sf::Vector2f getRotatedOffset(const sf::RectangleShape& rect)
  {
    float length = rect.getSize().x;
    float angleRad = rect.getRotation() * 3.14159265f / 180.f;
    return sf::Vector2f(cos(angleRad), sin(angleRad)) * length;
  }

  float distance(const sf::Vector2f& a, const sf::Vector2f& b)
  {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }

  float dot(sf::Vector2f a, sf::Vector2f b)
  {
    return a.x * b.x + a.y * b.y;
  }

  enum class ExtremumType
  {
    LeftmostUppermost,
    LeftmostDownmost,
    RightmostUppermost,
    RightmostDownmost,
    UpmostLeftmost,
    UpmostRightmost,
    DownmostLeftmost,
    DownmostRightmost
  };

  sf::Vector2f findExtremum(const std::vector<sf::Vector2f>& points, ExtremumType type)
  {
    return *std::min_element(points.begin(), points.end(),
                             [type](const sf::Vector2f& a, const sf::Vector2f& b)
                             {
                               switch (type)
                               {
                                 case ExtremumType::LeftmostUppermost:
                                   return (a.x != b.x) ? a.x < b.x : a.y > b.y;
                                 case ExtremumType::LeftmostDownmost:
                                   return (a.x != b.x) ? a.x < b.x : a.y < b.y;
                                 case ExtremumType::RightmostUppermost:
                                   return (a.x != b.x) ? a.x > b.x : a.y > b.y;
                                 case ExtremumType::RightmostDownmost:
                                   return (a.x != b.x) ? a.x > b.x : a.y < b.y;
                                 case ExtremumType::UpmostLeftmost:
                                   return (a.y != b.y) ? a.y > b.y : a.x < b.x;
                                 case ExtremumType::UpmostRightmost:
                                   return (a.y != b.y) ? a.y > b.y : a.x > b.x;
                                 case ExtremumType::DownmostLeftmost:
                                   return (a.y != b.y) ? a.y < b.y : a.x < b.x;
                                 case ExtremumType::DownmostRightmost:
                                   return (a.y != b.y) ? a.y < b.y : a.x > b.x;
                               }
                               return false;  // default, should never hit
                             });
  }
}  // namespace sfml_helpers
