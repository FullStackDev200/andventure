#ifndef ROOM_H
#define ROOM_H

#include <SFML/Graphics.hpp>
#include <array>

class Room : public sf::RectangleShape
{
 public:
  // Constructor that initializes the position and size of the rectangle
  Room(float X = 0.0f, float Y = 0.0f, float height = 0.0f, float width = 0.0f);

  // Function to get the center of the rectangle
  sf::Vector2f getCenter() const;

  // Function to get the surrounding walls (given a wall width)
  std::array<sf::Vector2f, 4> getSurroundWalls(int wallWidth);

 private:
  float X, Y, height, width;
};

#endif  // ROOM_H
