
#include "./src/adventure_graph.hpp"
#include "./src/rectangular_boundry_collision.hpp"
#include "./src/room.h"
#include "./src/sfml_helpers.hpp"

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
#include <cmath>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;
using namespace adventure_graph;
using namespace sfml_helpers;

vector<sf::RectangleShape> sf_paths;

std::vector<std::array<sf::Vector2f, 3>> makeNewPaths(std::vector<Room> rooms)
{
  std::vector<std::array<sf::Vector2f, 3>> newPaths;

  if (rooms.size() < 2)
  {                   // Add this check
    return newPaths;  // Return an empty vector if there are fewer than 2 rooms
  }

  std::vector<sf::Vector2f> roomsCenter(rooms.size());
  std::transform(rooms.begin(), rooms.end(), roomsCenter.begin(),
                 [](const sf::RectangleShape &room) { return sf::Vector2f(room.getPosition().x + room.getSize().x / 2, room.getPosition().y + room.getSize().y / 2); });

  for (size_t room = 0; room < rooms.size() - 1; room++)
  {
    sf::Vector2f startPoint = roomsCenter[room];
    sf::Vector2f middlePoint = sf::Vector2f(roomsCenter[room].x, roomsCenter[room + 1].y);
    sf::Vector2f endPoint = roomsCenter[room + 1];

    newPaths.push_back({startPoint, middlePoint, endPoint});
  }

  return newPaths;
}

vector<Room> vectorToRoom(const vector<pair<int, int>> &rooms, const vector<pair<int, int>> &coords)
{
  vector<Room> myRooms;
  for (int i = 0; i < rooms.size(); i++)
  {
    myRooms.push_back(Room(coords[i].first, coords[i].second, rooms[i].first, rooms[i].second));
  }
  return myRooms;
}

int main()
{
  build_graph();

  vector<pair<int, int>> coords = get_coordinates();
  vector<pair<int, int>> vecRooms = get_rooms();
  vector<Room> rooms = vectorToRoom(get_rooms(), get_coordinates());

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
    renderTexture.draw(rooms[i]);
    cout << "Windows drawn \n";
  }

  // Rooms
  /*vector<sf::RectangleShape> sf_rooms;*/

  /*for (int i = 0; i < rooms.size(); i++)*/
  /*{*/
  /*  sf::RectangleShape room;*/
  /**/
  /*  room.setSize(sf::Vector2f(rooms[i].first, rooms[i].second));*/
  /*  room.setFillColor(sf::Color::Blue);*/
  /**/
  /*  cout << "rooms cords" << "x " << coords[i].first << " y " << rooms[i].second;*/
  /*  room.setPosition(coords[i].first, window_height - rooms[i].second - coords[i].second);*/
  /*  sf_rooms.push_back(room);*/
  /*}*/
  /**/

  std::vector<std::array<sf::Vector2f, 3>> newPaths = makeNewPaths(rooms);
  for (const auto &path : newPaths)
  {
    sf::RectangleShape firstPath = getThickLine(window, path[0], path[1], sf::Color::Red, 1);
    sf::RectangleShape secondPath = getThickLine(window, path[1], path[2], sf::Color::Red, 1);
    sf_paths.push_back(firstPath);
    sf_paths.push_back(secondPath);

    renderTexture.draw(firstPath);
    renderTexture.draw(secondPath);
  }

  // Finalize the RenderTexture
  renderTexture.display();

  // Create a sprite from the RenderTexture
  sf::Sprite staticBackground(renderTexture.getTexture());

  // Player
  sf::RectangleShape player;
  player.setSize(sf::Vector2f(1, 1));         // Increase the size of the player
  player.setPosition(0, player.getSize().y);  // Move the player within the window
  player.setOutlineColor(sf::Color::Green);
  player.setFillColor(sf::Color::Green);

  float playerSpeed = 0.03;

  // Drawing static obects
  // Draw rooms
  for (const Room &room : rooms)
  {
    renderTexture.draw(room);
    cout << "Rooms drawn 2 \n";
  }

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
      player.move(0, playerSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player.getPosition().y + player.getSize().y < window_height)
    {
      player.move(0, -playerSpeed);
    }

    /*if (isOnWalkableArea(player, sf_rooms))*/
    /*{*/
    /*  std::cout << "Safe to move.\n";*/
    /*}*/
    /*if (std::any_of(rooms.begin(), rooms.end(), [&player](const Room &x) { return collision::areColliding(player, x); }))*/
    /*{*/
    /*  std::cout << "Safe to move.\n";*/
    /*}*/
    /*else if (std::any_of(sf_paths.begin(), sf_paths.end(), [&player](const Room &x) { return collision::areColliding(player, x, -1); }))*/
    /*{*/
    /*  std::cout << "Player is on the line.\n";*/
    /*}*/
    /*else*/
    /*{*/
    /*  std::cout << "Collision detected! Reverting movement.\n";*/
    /*  player.setPosition(originalPosition);  // Revert to the original position*/
    /*}*/
    /**/
    window.draw(player);
    window.display();
  }

  return 0;
}
