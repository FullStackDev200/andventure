#include "game.hpp"
#include <cstddef>
#include <iostream>
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "adventure_graph.hpp"
#include "path.h"
#include "player.h"
#include "rectangular_boundry_collision.hpp"
#include "room.h"
#include "sfml_helpers.hpp"

Game::Game()
{
  adventure_graph::build_graph();
  int scale = 10;

  int window_width = (adventure_graph::get_graph().size() - 1) * scale;
  int window_height = (adventure_graph::get_graph()[0].size() * scale);

  window.create(sf::VideoMode(window_width, window_height), "Andventure");

  // Set Coords System like Maths
  sf::View mathView(sf::FloatRect(0, 0, window_width, window_height));
  mathView.setCenter(window_width / 2.0f, window_height / 2.0f);
  mathView.setViewport(sf::FloatRect(0, 0, 1, 1));

  mathView.setSize(window_width, -window_height);

  window.setView(mathView);

  if (!renderTexture.create(window_width, window_height))
  {
    std::cerr << "Failed to create render texture!" << std::endl;
  }

  player.setSize(sf::Vector2(10.0f, 10.0f));
  player.setFillColor(sf::Color::Red);
  player.setSpeed(0.1);

  renderTexture.clear(sf::Color::Black);
}

void Game::processEvents()
{
  // Process window events
  sf::Event event;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
    {
      window.close();
    }
  }
}

int Game::run()
{
  sf::Clock clock;
  while (window.isOpen())
  {
    processEvents();
    float dt = clock.restart().asSeconds();
    update(dt);
    render();
  }
  return 0;
}

void Game::generatePaths()
{
  auto pathsCoords = sfml_helpers::makeNewPaths(rooms);

  for (const auto& pathCoord : pathsCoords)
  {
    Path path(pathCoord[2], pathCoord[1], pathCoord[0], scale, 1);

    renderTexture.draw(path);

    for (const auto& wall : path.getWalls()) renderTexture.draw(wall);

    paths.emplace_back(std::move(path));
  }
}

void Game::preRenderMap()
{
  renderTexture.clear(sf::Color::Black);

  generatePaths();

  // Draw walls and doors
  for (const auto& room : rooms)
  {
    renderTexture.draw(room);
  }

  for (const auto& wall : walls)
  {
    renderTexture.draw(wall);
  }

  renderTexture.display();
  staticBackground.setTexture(renderTexture.getTexture());
}

void Game::loadMap()
{
  auto coords = adventure_graph::get_coordinates();
  auto vecRooms = adventure_graph::get_rooms();
  scale = 10;

  rooms = sfml_helpers::vectorToRoom(vecRooms, coords, scale);

  // Set up walls from rooms
  // Set up walls from rooms
  for (const Room& room : rooms)
  {
    renderTexture.draw(room);
    std::array<sf::Vector2f, 4> edgePoints = room.getEdgePoints(scale);

    for (size_t i = 0; i < edgePoints.size(); i++)
    {
      sf::RectangleShape wall;
      if (i == edgePoints.size() - 1)  // Last wall, explicitly connect last to first
        wall = sfml_helpers::getRectagleWith2Vectors(edgePoints[i], edgePoints[0], scale);
      else
        wall = sfml_helpers::getRectagleWith2Vectors(edgePoints[i], edgePoints[i + 1], scale);

      wall.setFillColor(sf::Color::Magenta);
      walls.push_back(wall);
      renderTexture.draw(wall);
      cout << "Wall " << i << " drawn\n";
    }
  }
}

void Game::update(float dt)
{
  sf::Vector2f originalPos = player.getPosition();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    player.move(-player.getSpeed(), 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    player.move(player.getSpeed(), 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    player.move(0, player.getSpeed());
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    player.move(0, -player.getSpeed());

  // if (std::any_of(doors.begin(), doors.end(), [this](const sf::FloatRect& door) { return player.getGlobalBounds().intersects(door); }))
  // {
  //   // TODO: Make door logic here
  // }

  if (std::any_of(walls.begin(), walls.end(), [this](const sf::RectangleShape& wall) { return collision::areColliding(player, wall, -1); }))
  {
    player.setPosition(originalPos);
  }
}

void Game::render()
{
  window.clear(sf::Color::Black);
  window.draw(staticBackground);
  window.draw(player);
  window.display();
}
