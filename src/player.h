#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>

class Player : public sf::RectangleShape
{
 public:
  Player(float X = 0.0f, float Y = 0.0f, float height = 0.0f, float width = 0.0f);
  void setSpeed(float);
  float getSpeed();
  sf::Vector2f getCenter();
  void move(sf::Vector2f);
  std::array<sf::Vector2f, 4> getSurroundWalls();

 private:
  float X, Y, height, width, center, speed;
};

#endif  // PLAYER_H
