#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Player
{
 public:
  Player(float X = 0.0f, float Y = 0.0f, float height = 0.0f, float width = 0.0f);
  sf::Vector2f getCenter();
  sf::Vector2f getPosition();

 private:
  sf::RectangleShape rectangle;
  float X, Y, height, width, center;
};

#endif  // PLAYER_H
