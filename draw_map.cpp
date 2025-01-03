#include "./src/adventure_graph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;
using namespace adventure_graph;

// void drawLine(sf::RenderWindow &window, sf::Vector2i point1,sf::Vector2i point2, int lineWidth, sf::Color lineColor) {
//   int x0 = point1.x;
//   int y0 = point1.y;
//   int x1 = point2.x;
//   int y1 = point2.y;
//   int dx = abs(x1 - x0);
//   int sx = (x0 < x1) ? 1 : -1;
//   int dy = -abs(y1 - y0);
//   int sy = (y0 < y1) ? 1 : -1;
//   int err = dx + dy;
//   while (true) {
//     sf::RectangleShape rect(sf::Vector2f(lineWidth, lineWidth));
//     rect.setFillColor(lineColor);
//     rect.setPosition(x0, y0);
//     window.draw(rect);
//     if (x0 == x1 && y0 == y1)
//       break;
//     int e2 = 2 * err;
//     if (e2 >= dy) {
//       err += dy;
//       x0 += sx;
//     }
//     if (e2 <= dx) {
//       err += dx;
//       y0 += sy;
//     }
//   }
// }

auto get_formula(pair<float, float> middle1, pair<float, float> middle2)
{
  return [middle1, middle2](int x)
  {
    return (middle2.first - middle1.first) / (middle2.second - middle1.second) * (x - middle1.second);
  };
}

void drawLine(sf::RenderWindow &window, pair<float, float> middle1, pair<float, float> middle2, sf::Color lineColor, int window_height)
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

int main()
{

  build_graph();

  vector<pair<int, int>> rooms = get_rooms();
  vector<pair<int, int>> coords = get_coordinates();
  vector<pair<pair<int, int>, pair<int, int>>> middles = get_middles();

  int window_width = (get_graph().size() - 1);
  int window_height = (get_graph()[0].size());

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "SFML Window");

  sf::VertexArray path_points(sf::Points);

  vector<sf::RectangleShape> rectangles;
  cout << "before loop" << endl;

  // Rectangles
  for (int i = 0; i < rooms.size(); i++)
  {

    sf::RectangleShape rectangle;

    rectangle.setSize(
        sf::Vector2f(rooms[i].first,
                     rooms[i].second)); // Make sure these are reasonable
    // values for width and height
    rectangle.setFillColor(sf::Color::Blue); // Fill color
    /*rectangle.setOutlineThickness(5);          // Outline thickness*/
    /*rectangle.setOutlineColor(sf::Color::Red); // Outline color*/
    cout << "rooms cords" << "x " << coords[i].first << " y "
         << rooms[i].second;
    rectangle.setPosition(coords[i].first,
                          window_height - rooms[i].second - coords[i].second);
    rectangles.push_back(rectangle);
  }

  // Paths
  // vector<std::array<sf::Vertex, 2>> paths;

  // for (int i = 0; i < rooms.size() - 1; i++)
  // {
  //   std::array<sf::Vertex, 2> path = {
  //       sf::Vertex(sf::Vector2f(middles[i].first.first,
  //                               window_height - middles[i].first.second),
  //                  sf::Color::Red),
  //       sf::Vertex(sf::Vector2f(middles[i].second.first,
  //                               window_height - middles[i].second.second),
  //                  sf::Color::Red)};
  //   paths.push_back(path);
  // }
  // Start the SFML clock for frame timing
  sf::Clock clock;
  while (window.isOpen())
  {
    sf::Time deltaTime = clock.restart();
    float dt = deltaTime.asSeconds();

    // Process window events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape))
      {
        window.close();
      }
    }

    // Clear the window
    window.clear(sf::Color::Black);

    // Draw rectangles
    for (const auto &rectangle : rectangles)
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

    window.display();
  }

  // Ensure return outside the loop
  return 0;
}
