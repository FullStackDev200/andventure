#include "game.hpp"
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
#include "room.h"
#include "sfml_helpers.hpp"

Game::Game()
{
  adventure_graph::build_graph();
  vector<pair<int, int>> coords = adventure_graph::get_coordinates();
  vector<pair<int, int>> vecRooms = adventure_graph::get_rooms();
  int scale = 10;

  vector<Room> rooms = sfml_helpers::vectorToRoom(adventure_graph::get_rooms(), adventure_graph::get_coordinates(), scale);

  int window_width = (adventure_graph::get_graph().size() - 1) * scale;
  int window_height = (adventure_graph::get_graph()[0].size() * scale);

  window.create(sf::VideoMode(window_width, window_height), "Andventure");

  // Set Coords System like Maths
  sf::View mathView(sf::FloatRect(0, 0, window_width, window_height));
  mathView.setCenter(window_width / 2.0f, window_height / 2.0f);
  mathView.setViewport(sf::FloatRect(0, 0, 1, 1));

  // Flip Y by scaling -1 and translating
  mathView.setSize(window_width, -window_height);  // Negative height flips Y

  window.setView(mathView);

  // Create a RenderTexture to draw rooms and paths once
  if (!renderTexture.create(window_width, window_height))
  {
    std::cerr << "Failed to create render texture!" << std::endl;
  }
  player.setSize(sf::Vector2(10.0f, 10.0f));
  player.setFillColor(sf::Color::Red);
  player.setSpeed(1.0);

  // Pre-render rooms and paths to the RenderTexture
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

void Game::makePaths()
{
  std::vector<std::array<sf::Vector2f, 3>> pathsCoords = sfml_helpers::makeNewPaths(rooms);

  for (const auto& pathCoord : pathsCoords)
  {
    Path path(pathCoord[2], pathCoord[1], pathCoord[0], scale, 1);
    path.setwallWidth(1);
    paths.emplace_back(path);
  }
}

void Game::makePathWalls()
{
  for (const auto& path : paths)
  {
    const auto& walls = path.getWalls();

    sf::FloatRect interseciton1;
    sf::FloatRect interseciton2;
    path.getLine1().getGlobalBounds().intersects(path.getLine1().getGlobalBounds(), interseciton1);
    path.getLine2().getGlobalBounds().intersects(path.getLine2().getGlobalBounds(), interseciton2);
    doors.emplace_back(interseciton1);
    doors.emplace_back(interseciton2);

    for (const auto& wall : walls)
    {
      renderTexture.draw(wall);
    }
  }
}

void Game::preRenderMap()
{
  makePaths();
  makePathWalls();

  renderTexture.clear(sf::Color::Black);

  // Draw walls and doors
  for (const auto& room : rooms)
  {
    renderTexture.draw(room);
  }

  for (const auto& path : paths)
  {
    renderTexture.draw(path);
  }

  for (const auto& wall : walls)
  {
    renderTexture.draw(wall);
  }
  for (const auto& pathWall : pathWalls)
  {
    renderTexture.draw(pathWall);
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
  for (const Room& room : rooms)
  {
    auto edges = room.getEdgePoints(scale);
    for (size_t i = 0; i < edges.size(); ++i)
    {
      auto p1 = edges[i];
      auto p2 = edges[(i + 1) % edges.size()];
      auto wall = sfml_helpers::getRectagleWith2Vectors(p1, p2, scale);
      wall.setFillColor(sf::Color::Magenta);
      walls.emplace_back(wall);
    }
  }

  // Setup paths and doors
  auto pathsCoords = sfml_helpers::makeNewPaths(rooms);

  for (const auto& coords : pathsCoords)
  {
    Path path(coords[2], coords[1], coords[0], scale, 1);
    path.setwallWidth(1);
    pathWalls = path.getWalls();
    for (const auto& wall : pathWalls) renderTexture.draw(wall);

    sf::FloatRect inter1, inter2;
    path.getLine1().getGlobalBounds().intersects(path.getLine1().getGlobalBounds(), inter1);
    path.getLine2().getGlobalBounds().intersects(path.getLine2().getGlobalBounds(), inter2);
    doors.emplace_back(inter1);
    doors.emplace_back(inter2);
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
  //
  // if (std::any_of(walls.begin(), walls.end(), [this](const sf::RectangleShape& wall) { return collision::areColliding(player, wall, -1); }))
  // {
  //   player.setPosition(originalPos);
  // }
}

void Game::render()
{
  window.clear(sf::Color::Black);
  window.draw(staticBackground);
  player.setFillColor(sf::Color::Red);
  window.draw(player);
  window.display();
}
