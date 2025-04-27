
#ifndef PATH_H
#define PATH_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class Path : public sf::Drawable
{
 public:
  Path(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3, int width);

  sf::Vector2f getPoint1();
  sf::Vector2f getPoint2();
  sf::Vector2f getPoint3();

  sf::RectangleShape getWalls();

 private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  sf::Vector2f point1;
  sf::Vector2f point2;
  sf::Vector2f point3;

  sf::RectangleShape line1, line2;
};

#endif  // PATH_H
