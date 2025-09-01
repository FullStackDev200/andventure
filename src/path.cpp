
#include "path.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cwchar>
#include <vector>
#include "SFML/Graphics/Color.hpp"
#include "sfml_helpers.hpp"

Path::Path(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, int width, int wallWidth) : point1(p1), point2(p2), point3(p3), width(width), wallWidth(wallWidth)
{
  line1 = sfml_helpers::getThickLine(point1, point2, sf::Color::Red, width);

  line2 = sfml_helpers::getRectagleWith2Vectors(point3, point2, width);
  line2.setFillColor(sf::Color::Red);
}

const sf::Vector2f Path::getPoint1()
{
  return point1;
}
const sf::Vector2f Path::getPoint2()
{
  return point2;
}
const sf::Vector2f Path::getPoint3()
{
  return point3;
}

std::vector<sf::RectangleShape> Path::getWalls() const
{
  float offset_distance = (width / 2.0f) + wallWidth;

  auto unitVector = [](sf::Vector2f v) -> sf::Vector2f
  {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return len != 0 ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0, 0);
  };

  auto perpendicular = [](sf::Vector2f v) -> sf::Vector2f { return sf::Vector2f(-v.y, v.x); };

  // Direction vectors
  sf::Vector2f dir1 = point2 - point1;
  sf::Vector2f dir2 = point3 - point2;

  sf::Vector2f normal1 = perpendicular(unitVector(dir1));
  sf::Vector2f normal2 = perpendicular(unitVector(dir2));

  // Compute miter (bisector) at point2
  sf::Vector2f bisector = unitVector(normal1 + normal2);
  float scale = offset_distance / std::max(0.1f, std::sqrt((1.0f + sfml_helpers::dot(normal1, normal2)) / 2.0f));  // optional safety clamp

  sf::Vector2f joinOffset = bisector * scale;

  // Compute wall edges
  sf::Vector2f wall1_p1 = point1 + normal1 * offset_distance;
  sf::Vector2f wall1_p2 = point2 + joinOffset;

  sf::Vector2f wall2_p1 = point2 + joinOffset;
  sf::Vector2f wall2_p2 = point3 + normal2 * offset_distance;

  sf::Vector2f wall3_p1 = point1 - normal1 * offset_distance;
  sf::Vector2f wall3_p2 = point2 - joinOffset;

  sf::Vector2f wall4_p1 = point2 - joinOffset;
  sf::Vector2f wall4_p2 = point3 - normal2 * offset_distance;

  // Create rectangles
  sf::RectangleShape wall1 = sfml_helpers::getThickLine(wall1_p1, wall1_p2, sf::Color::Yellow, wallWidth);
  sf::RectangleShape wall2 = sfml_helpers::getThickLine(wall2_p1, wall2_p2, sf::Color::Yellow, wallWidth);
  sf::RectangleShape wall3 = sfml_helpers::getThickLine(wall3_p1, wall3_p2, sf::Color::Yellow, wallWidth);
  sf::RectangleShape wall4 = sfml_helpers::getThickLine(wall4_p1, wall4_p2, sf::Color::Yellow, wallWidth);

  return {wall1, wall2, wall3, wall4};
}

void Path::drawWalls(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(wall1);
  target.draw(wall2);
  target.draw(wall3);
  target.draw(wall4);
}

void Path::setwallWidth(int newWallWidth)
{
  wallWidth = newWallWidth;
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(line1, states);
  target.draw(line2, states);
}

const sf::RectangleShape& Path::getLine1() const
{
  return line1;
}

const sf::RectangleShape& Path::getLine2() const
{
  return line2;
}
