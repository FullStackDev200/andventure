
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

vector<sf::RectangleShape> sf_paths;

auto get_formula(pair<float, float> &middle1, pair<float, float> &middle2)
{
  return [middle1, middle2](int x) { return (middle2.first - middle1.first) / (middle2.second - middle1.second) * (x - middle1.second); };
}

bool isContained(sf::RectangleShape &player, vector<sf::RectangleShape> &rooms)
{
  // Get the global bounds of both rectangles
  sf::FloatRect innerBounds = player.getGlobalBounds();

  bool isWalkable = false;

  for (auto room : rooms)
  {
    sf::FloatRect outerBounds = room.getGlobalBounds();
    if (innerBounds.left >= outerBounds.left && innerBounds.top >= outerBounds.top && innerBounds.left + innerBounds.width <= outerBounds.left + outerBounds.width &&
        innerBounds.top + innerBounds.height <= outerBounds.top + outerBounds.height)
    {
      isWalkable = true;
      break;
    }
  }

  return isWalkable;
}

bool isCollidingWithLine(const sf::RectangleShape &player, const sf::RectangleShape &line)
{
  // Get player's bounding box
  sf::FloatRect playerBounds = player.getGlobalBounds();

  // Get the four corners of the line after applying transformations
  std::vector<sf::Vector2f> linePoints(4);
  linePoints[0] = line.getTransform().transformPoint(0.f, 0.f);                            // Top-left
  linePoints[1] = line.getTransform().transformPoint(line.getSize().x, 0.f);               // Top-right
  linePoints[2] = line.getTransform().transformPoint(line.getSize().x, line.getSize().y);  // Bottom-right
  linePoints[3] = line.getTransform().transformPoint(0.f, line.getSize().y);               // Bottom-left

  // Check if any of the line's transformed points intersects the player bounds
  for (const auto &point : linePoints)
  {
    if (playerBounds.contains(point))
      return true;
  }

  // You can also check for intersections between the edges of the line and player bounds if needed
  return false;
}

sf::RectangleShape getThickLine(sf::RenderWindow &window, sf::Vector2f point1, sf::Vector2f point2, sf::Color lineColor, float thickness)
{
  // Calculate the direction and length of the line
  sf::Vector2f direction = point2 - point1;
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

  // Normalize the direction vector
  direction /= length;

  // Create a rectangle shape to represent the line with the desired thickness
  sf::RectangleShape line(sf::Vector2f(length, thickness));
  line.setFillColor(lineColor);

  // Set the orientation of the rectangle to match the line's direction
  line.setPosition(point1);
  float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;  // Convert to degrees
  line.setRotation(angle);

  // Draw the line (rectangle)
  return line;
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

  // Create a RenderTexture to draw rooms and paths once
  sf::RenderTexture renderTexture;
  if (!renderTexture.create(window_width, window_height))
  {
    cerr << "Failed to create render texture!" << endl;
    return -1;
  }

  // Pre-render rooms and paths to the RenderTexture
  renderTexture.clear(sf::Color::Black);

  // Draw rooms
  for (int i = 0; i < rooms.size(); i++)
  {
    sf::RectangleShape room;
    room.setSize(sf::Vector2f(rooms[i].first, rooms[i].second));
    room.setFillColor(sf::Color::Blue);
    room.setPosition(coords[i].first, window_height - rooms[i].second - coords[i].second);

    renderTexture.draw(room);
  }

  // Draw paths
  for (const auto &middle : middles)
  {
    sf::Vector2f start(middle.first.first, window_height - middle.first.second);
    sf::Vector2f end(middle.second.first, window_height - middle.second.second);
    sf::RectangleShape path = getThickLine(window, start, end, sf::Color::Red, 5);

    renderTexture.draw(path);
  }

  // Finalize the RenderTexture
  renderTexture.display();

  // Create a sprite from the RenderTexture
  sf::Sprite staticBackground(renderTexture.getTexture());
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
  player.setSize(sf::Vector2f(1, 1));                         // Increase the size of the player
  player.setPosition(0, window_height - player.getSize().y);  // Move the player within the window
  player.setOutlineColor(sf::Color::Green);
  player.setFillColor(sf::Color::Green);

  float playerSpeed = 0.03;

  // Drawing static obects
  // Draw rectangles

  for (const auto &rectangle : sf_rooms)
  {
    window.draw(rectangle);
  }

  // Draw lines using paths
  for (const auto middle : middles)
  {
    sf::Vector2f start(middle.first.first, window_height - middle.first.second);
    sf::Vector2f end(middle.second.first, window_height - middle.second.second);
    window.draw(getThickLine(window, start, end, sf::Color::Red, 5));
    sf_paths.push_back(getThickLine(window, start, end, sf::Color::Red, 5));
  }

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

    window.draw(staticBackground);

    // Player movement

    sf::Vector2f originalPosition = player.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0)
    {
      player.move(-playerSpeed, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x + player.getSize().x < window_width)
    {
      player.move(playerSpeed, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player.getPosition().y > 0)
    {
      player.move(0, -playerSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getPosition().y + player.getSize().y < window_height)
    {
      player.move(0, playerSpeed);
    }

    bool isOnLine = false;
    for (const auto &line : sf_paths)
    {
      if (isCollidingWithLine(player, line))
      {
        isOnLine = true;
        break;
      }
    }

    if (is_on_walkable_area(player, sf_rooms))
    {
      std::cout << "Safe to move.\n";
    }
    else if (is_on_walkable_area(player, sf_paths))
    {
      std::cout << "Player is on the line.\n";
    }
    else
    {
      std::cout << "Collision detected! Reverting movement.\n";
      player.setPosition(originalPosition);  // Revert to the original position
    }

    window.draw(player);

    window.display();
  }

  return 0;
}
