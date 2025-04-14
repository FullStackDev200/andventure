#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include "room.h"

using namespace std;

namespace sfml_helpers
{

  bool isContained(const sf::RectangleShape &player, const sf::RectangleShape &room);

  sf::RectangleShape getThickLine(sf::RenderWindow &window, sf::Vector2f point1, sf::Vector2f point2, sf::Color lineColor, float thickness);

  bool isOnWalkableArea(sf::RectangleShape player, vector<Room> shapes);

  sf::RectangleShape getRectagleWith2Vectors(const sf::Vector2f &point1, const sf::Vector2f &point2, float wallWidth);

}  // namespace sfml_helpers
