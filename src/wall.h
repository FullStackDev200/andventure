#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>
class Wall : public sf::RectangleShape
{
 public:
  Wall(sf::Vector2f start = {0.f, 0.f}, sf::Vector2f end = {0.f, 0.f}, float thickness = 0.f);
  std::vector<Wall> split(sf::FloatRect doorway) const;
  float getX() const;
  float getY() const;
  float getWidth() const;
  float getHeight() const;

 private:
  sf::Vector2f start, end;
  float x, y, width, heigh;
};

#endif  // !WALL_H
