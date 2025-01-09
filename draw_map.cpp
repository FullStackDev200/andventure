
#include "./src/adventure_graph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;
using namespace adventure_graph;

auto get_formula(pair<float, float> &middle1, pair<float, float> &middle2)
{
  return [middle1, middle2](int x) { return (middle2.first - middle1.first) / (middle2.second - middle1.second) * (x - middle1.second); };
}

void drawLine(sf::RenderWindow &window, pair<float, float> &middle1, pair<float, float> &middle2, sf::Color lineColor, int &window_height)
{
  sf::RectangleShape rect(sf::Vector2f(1, 1));
  rect.setFillColor(lineColor);
  auto formula = get_formula(middle1, middle2);

  if (middle2.second == middle1.second)
  {
    for (size_t x = min(middle1.first, middle2.first); x <= max(middle1.first, middle2.first); x++)
    {
      rect.setPosition(x, window_height - middle1.second);
      window.draw(rect);
      rect.setPosition(x, window_height - middle1.second - 1);
      window.draw(rect);
    }
    return;
  }

  for (size_t x = min(middle1.second, middle2.second); x <= max(middle1.second, middle2.second); x++)
  {
    int form = formula(x);

    rect.setPosition(middle1.first + form, window_height - x);
    window.draw(rect);
    rect.setPosition(middle1.first + form, window_height - x - 1);
    window.draw(rect);
    rect.setPosition(middle1.first + form - 1, window_height - x);
    window.draw(rect);
    rect.setPosition(middle1.first + form - 1, window_height - x - 1);
    window.draw(rect);

    if (form - formula(x - 1) > 1 && x != min(middle1.second, middle2.second))
    {
      for (size_t i = 2; i <= form - formula(x - 1); i++)
      {
        rect.setPosition(middle1.first + form - i, window_height - x);
        window.draw(rect);
        rect.setPosition(middle1.first + form - i, window_height - x - 1);
        window.draw(rect);
      }
    }
    else if (formula(x - 1) - form > 1 && x != max(middle1.second, middle2.second))
    {
      for (size_t i = 2; i <= formula(x - 1) - form; i++)
      {
        rect.setPosition(middle1.first + form - i, window_height - x);
        window.draw(rect);
        rect.setPosition(middle1.first + form - i, window_height - x - 1);
        window.draw(rect);
      }
    }
  }
}

bool is_on_walkable_area(sf::RectangleShape player, vector<sf::RectangleShape> rooms)
{
  sf::FloatRect playerBounds = player.getGlobalBounds();

  bool isWalkable = false;
  for (const auto &shape : rooms)
  {
    if (shape.getGlobalBounds().intersects(playerBounds))
    {
      isWalkable = true;
      break;
    }
  }
  return isWalkable;
}

bool isColliding(const sf::RectangleShape &rect1, const sf::RectangleShape &rect2)
{
  return rect1.getGlobalBounds().intersects(rect2.getGlobalBounds());
}

int main()
{
  build_graph();

  vector<pair<int, int>> rooms = get_rooms();
  vector<pair<int, int>> coords = get_coordinates();
  vector<pair<pair<int, int>, pair<int, int>>> middles = get_middles();

  int window_width = (get_graph().size() - 1);
  int window_height = (get_graph()[0].size());

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML Window");

  sf::VertexArray path_points(sf::Points);

  // Rooms

  vector<sf::RectangleShape> sf_rooms;

  for (int i = 0; i < rooms.size(); i++)
  {
    sf::RectangleShape room;

    room.setSize(sf::Vector2f(rooms[i].first, rooms[i].second));
    room.setFillColor(sf::Color::Blue);

    cout << "rooms cords" << "x " << coords[i].first << " y " << rooms[i].second;
    room.setPosition(coords[i].first, window_height - rooms[i].second - coords[i].second);
    sf_rooms.push_back(room);
  }

  // Player
  sf::RectangleShape player;
  player.setSize(sf::Vector2f(5, 5));         // Increase the size of the player
  player.setPosition(0, window_height - 10);  // Move the player within the window
  player.setOutlineColor(sf::Color::Green);
  player.setFillColor(sf::Color::Green);

  float playerSpeed = 1;

  sf::Clock clock;
  while (window.isOpen())
  {
    sf::Time deltaTime = clock.restart();
    float dt = deltaTime.asSeconds();

    // Process window events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
      {
        window.close();
      }
    }

    // Clear the window
    window.clear(sf::Color::Black);

    // Draw rectangles
    for (const auto &rectangle : sf_rooms)
    {
      window.draw(rectangle);
    }
    // Draw lines using paths
    for (const auto middle : middles)
    {
      pair<float, float> start(middle.first.first, middle.first.second);
      pair<float, float> end(middle.second.first, middle.second.second);
      drawLine(window, start, end, sf::Color::Red, window_height);
    }

    // Player movement

    sf::Vector2f originalPosition = player.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0 && is_on_walkable_area(player, sf_rooms))
    {
      player.move(-playerSpeed, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x + player.getSize().x < window_width && is_on_walkable_area(player, sf_rooms))
    {
      player.move(playerSpeed, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player.getPosition().y > 0 && is_on_walkable_area(player, sf_rooms))
    {
      player.move(0, -playerSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getPosition().y + player.getSize().y < window_height && is_on_walkable_area(player, sf_rooms))
    {
      player.move(0, playerSpeed);
    }

    if (!is_on_walkable_area(player, sf_rooms))
    {
      std::cout << "Collision detected! Reverting movement.\n";
      player.setPosition(originalPosition);  // Revert to the original position
    }
    else
    {
      std::cout << "Safe to move.\n";
    }

    window.draw(player);

    window.display();
  }

  return 0;
}
