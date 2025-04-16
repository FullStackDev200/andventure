
#include "path.h"
#include <bits/types/wint_t.h>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include "sfml_helpers.hpp"

Path::Path(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, int width) : point1(p1), point2(p2), point3(p3)
{
  line1 = sfml_helpers::getThickLine(point1, point2, sf::Color::Red, width);

  line2 = sfml_helpers::getRectagleWith2Vectors(point3, point2, width);
  line2.setFillColor(sf::Color::Red);
}

sf::Vector2f Path::getPoint1()
{
  return point1;
}
sf::Vector2f Path::getPoint2()
{
  return point2;
}
sf::Vector2f Path::getPoint3()
{
  return point3;
}

sf::RectangleShape Path::getWalls()
{
  return sf::RectangleShape();  // dummy for now
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(line1, states);
  target.draw(line2, states);
}
