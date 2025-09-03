
#ifndef PATH_H
#define PATH_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>

class Path : public sf::Drawable
{
 public:
  Path(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, int width, int wallWidth);

  const sf::Vector2f getPoint1();
  const sf::Vector2f getPoint2();
  const sf::Vector2f getPoint3();

  const sf::RectangleShape& getLine1() const;
  const sf::RectangleShape& getLine2() const;

  const std::array<sf::RectangleShape, 4> getWalls() const;

  void setWallWidth(int newWallWidth);
  void drawWalls(sf::RenderTarget& target) const;
  void recalculateWalls();

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  sf::Vector2f point1;
  sf::Vector2f point2;
  sf::Vector2f point3;
  int width;
  int wallWidth;

  void generateWalls();

  sf::RectangleShape line1, line2;
  sf::RectangleShape wall1, wall2, wall3, wall4;
  std::array<sf::RectangleShape, 4> walls;
};

#endif  // PATH_H
