#include "wall.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "sfml_helpers.hpp"

Wall::Wall(sf::Vector2f start, sf::Vector2f end, float thickness) : start(start), end(end)
{
  sf::Vector2f direction = end - start;
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

  sf::Vector2f unitDirection = direction / length;

  this->setSize({length, thickness});
  this->setFillColor(sf::Color::Magenta);

  this->setOrigin(0, thickness / 2.0f);

  this->setPosition(start);
  float angle = std::atan2(unitDirection.y, unitDirection.x) * 180.0f / 3.14159f;
  this->setRotation(angle);
}

std::vector<Wall> Wall::split(sf::FloatRect doorway) const
{
  std::vector<Wall> result;

  if (!doorway.intersects(this->getGlobalBounds()))
  {
    result.push_back(*this);
    return result;
  }

  sf::FloatRect intersection;
  doorway.intersects(this->getGlobalBounds(), intersection);

  bool isHorizontal = getSize().x > getSize().y;

  if (isHorizontal)
  {
    float y = getPosition().y;

    sf::Vector2f leftStart(getPosition().x, y);
    sf::Vector2f leftEnd(intersection.left, y);

    sf::Vector2f rightStart(intersection.left + intersection.width, y);
    sf::Vector2f rightEnd(getPosition().x + getSize().x, y);

    if (leftEnd.x > leftStart.x)
      result.emplace_back(leftStart, leftEnd, getSize().y);
    if (rightEnd.x > rightStart.x)
      result.emplace_back(rightStart, rightEnd, getSize().y);
  }
  else
  {
    float x = getPosition().x;

    sf::Vector2f topStart(x, getPosition().y);
    sf::Vector2f topEnd(x, intersection.top);

    sf::Vector2f bottomStart(x, intersection.top + intersection.height);
    sf::Vector2f bottomEnd(x, getPosition().y + getSize().y);

    if (topEnd.y > topStart.y)
      result.emplace_back(topStart, topEnd, getSize().x);
    if (bottomEnd.y > bottomStart.y)
      result.emplace_back(bottomStart, bottomEnd, getSize().x);
  }

  return result;
}

float Wall::getX() const
{
  return this->getPosition().x;
}
float Wall::getY() const
{
  return this->getPosition().y;
}
float Wall::getWidth() const
{
  return this->getSize().x;
}
float Wall::getHeight() const
{
  return this->getSize().y;
}
