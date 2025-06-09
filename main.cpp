#include "./src/adventure_graph.hpp"

#include "./src/rectangular_boundry_collision.hpp"
#include "./src/room.h"
#include "./src/sfml_helpers.hpp"
#include "src/path.h"
#include "src/player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;
using namespace adventure_graph;
using namespace sfml_helpers;

int main()
{
  build_graph();

  vector<pair<int, int>> coords = get_coordinates();
  vector<pair<int, int>> vecRooms = get_rooms();

  int scale = 10;

  vector<Room> rooms = vectorToRoom(get_rooms(), get_coordinates(), scale);

  int window_width = (get_graph().size() - 1) * scale;
  int window_height = (get_graph()[0].size() * scale);

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML Window");

  // Create a RenderTexture to draw rooms and paths once
  sf::RenderTexture renderTexture;
  if (!renderTexture.create(window_width, window_height))
  {
    cerr << "Failed to create render texture!" << endl;
    return -1;
  }

  // Pre-render rooms and paths to the RenderTexture
  renderTexture.clear(sf::Color::Black);

  // Draw Paths
  std::vector<std::array<sf::Vector2f, 3>> pathsCoords = makeNewPaths(rooms);
  vector<Path> paths;

  for (const auto& pathCoord : pathsCoords)
  {
    Path path(pathCoord[2], pathCoord[1], pathCoord[0], scale, 1);
    path.setwallWidth(1);
    paths.push_back(path);

    const auto& walls = path.getWalls();

    for (const auto& wall : walls)
    {
      renderTexture.draw(wall);
    }
    // renderTexture.draw(path);
  }

  std::vector<sf::RectangleShape> walls;

  for (const Room& room : rooms)
  {
    renderTexture.draw(room);
    std::array<sf::Vector2f, 4> edgePoints = room.getEdgePoints(scale);

    for (int i = 0; i < edgePoints.size(); i++)
    {
      sf::RectangleShape wall;
      if (i == edgePoints.size() - 1)  // Last wall, explicitly connect last to first
        wall = getRectagleWith2Vectors(edgePoints[i], edgePoints[0], scale);
      else
        wall = getRectagleWith2Vectors(edgePoints[i], edgePoints[i + 1], scale);

      wall.setFillColor(sf::Color::Magenta);
      walls.push_back(wall);
      renderTexture.draw(wall);
      cout << "Wall " << i << " drawn\n";
    }
  }

  // Finalize the RenderTexture
  renderTexture.display();

  // Create a sprite from the RenderTexture
  sf::Sprite staticBackground(renderTexture.getTexture());

  // Player
  Player player;
  player.setSize(sf::Vector2f(1 * scale, 1 * scale));  // Increase the size of the player
  player.setPosition(scale, player.getSize().y);       // Move the player within the window
  player.setOutlineColor(sf::Color::Blue);
  player.setFillColor(sf::Color::Blue);
  player.setSpeed(0.01 * scale);

  // Set Coords System like Maths
  sf::View mathView(sf::FloatRect(0, 0, window_width, window_height));
  mathView.setCenter(window_width / 2.0f, window_height / 2.0f);
  mathView.setViewport(sf::FloatRect(0, 0, 1, 1));

  // Flip Y by scaling -1 and translating
  mathView.setSize(window_width, -window_height);  // Negative height flips Y

  window.setView(mathView);

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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      player.move(-player.getSpeed(), 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      player.move(player.getSpeed(), 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      player.move(0, player.getSpeed());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      player.move(0, -player.getSpeed());

    if (std::any_of(walls.begin(), walls.end(), [&player](const sf::RectangleShape& wall) { return collision::areColliding(player, wall, -1); }))
    {
      player.setPosition(originalPosition);
    }

    window.draw(player);
    window.display();
  }

  return 0;
}
