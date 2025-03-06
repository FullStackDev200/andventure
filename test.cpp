#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <iostream>
#include <vector>

std::vector<std::array<sf::Vector2f, 3>> makeNewPaths(std::vector<sf::RectangleShape> rooms)
{
  std::vector<std::array<sf::Vector2f, 3>> newPaths;

  if (rooms.size() < 2)
  {
    std::cout << "Not enough rooms to create paths. Returning empty vector.\n";
    return newPaths;  // Return an empty vector if there are fewer than 2 rooms
  }

  std::vector<sf::Vector2f> roomsCenter(rooms.size());
  std::transform(rooms.begin(), rooms.end(), roomsCenter.begin(),
                 [](const sf::RectangleShape& room)
                 {
                   sf::Vector2f center(room.getPosition().x + room.getSize().x / 2, room.getPosition().y + room.getSize().y / 2);
                   std::cout << "Room center: (" << center.x << ", " << center.y << ")\n";  // Print room center
                   return center;
                 });

  for (size_t room = 0; room < rooms.size() - 1; room++)
  {
    sf::Vector2f startPoint = roomsCenter[room];
    sf::Vector2f middlePoint = sf::Vector2f(roomsCenter[room + 1].x, roomsCenter[room + 1].y);
    sf::Vector2f endPoint = roomsCenter[room + 1];

    std::cout << "Path from (" << startPoint.x << ", " << startPoint.y << ") to (" << middlePoint.x << ", " << middlePoint.y << ") to (" << endPoint.x << ", " << endPoint.y
              << ")\n";  // Print the path

    newPaths.push_back({startPoint, middlePoint, endPoint});
  }

  return newPaths;
}

int main()
{
  // Create a SFML window
  sf::RenderWindow window(sf::VideoMode(800, 600), "Room Paths");

  // Define two rooms (Rectangles)
  sf::RectangleShape test1(sf::Vector2f(100, 100));
  test1.setPosition(150, 150);
  test1.setFillColor(sf::Color::Green);

  sf::RectangleShape test2(sf::Vector2f(100, 100));
  test2.setPosition(400, 300);
  test2.setFillColor(sf::Color::Blue);

  std::vector<sf::RectangleShape> rooms = {test1, test2};

  // Create paths based on the rooms
  std::vector<std::array<sf::Vector2f, 3>> paths = makeNewPaths(rooms);

  // Create a line for each path
  sf::VertexArray lines(sf::LinesStrip);

  // Add rooms to the window and paths between them
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Clear the window
    window.clear(sf::Color::White);

    // Draw the rooms
    window.draw(test1);
    window.draw(test2);

    // Draw the paths
    for (const auto& path : paths)
    {
      // Draw a line from start to middle to end
      lines.clear();
      lines.append(sf::Vertex(path[0], sf::Color::Red));     // Start point
      lines.append(sf::Vertex(path[1], sf::Color::Yellow));  // Middle point
      lines.append(sf::Vertex(path[2], sf::Color::Blue));    // End point
      window.draw(lines);
    }

    // Display everything in the window
    window.display();
  }

  return 0;
}
