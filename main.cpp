#include "./src/adventure_graph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

using namespace std;
using namespace adventure_graph;

int main() {

  build_graph();

  vector<pair<int, int>> rooms = get_rooms();
  vector<pair<int, int>> coords = get_coordinates();
  vector<pair<pair<int,int>, pair<int, int>>> middles = get_middles();

  int window_width = (get_graph().size() - 1);
  int window_height = (get_graph()[0].size());

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                          "SFML Window");

  sf::VertexArray path_points(sf::Points);

  vector<sf::RectangleShape> rectangles;
  cout << "before loop" << endl;



  //Rectangles
  for (int i = 0; i < rooms.size(); i++) {

    sf::RectangleShape rectangle;

    rectangle.setSize(
      sf::Vector2f(rooms[i].first,
                   rooms[i].second));      // Make sure these are reasonable
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
  vector<sf::Vertex *> paths;

  for (int i = 0; i < rooms.size() - 1; i++) {
    sf::Vertex path[] = {
      sf::Vertex(sf::Vector2f(
        middles[i].first.first, middles[i].first.second)),
      sf::Vertex(sf::Vector2f(
        middles[i].second.first, middles[i].second.second))};
    paths.push_back(path);
  }

  // Start the SFML clock for frame timing
  sf::Clock clock;



  while (window.isOpen()) {
    // Frame timing
    sf::Time deltaTime = clock.restart();
    float dt = deltaTime.asSeconds();

    // Process window events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }
    }

    // Clear the window and draw the rectangle
    window.clear(sf::Color::Black);
    for (int i = 0; i < rectangles.size() - 1; i++) {
      window.draw(rectangles[i]);
    }

    for (int i = 0; i < paths.size(); i++) {
      window.draw(paths[i], paths.size(), sf::LinesStrip);
    }
    window.display();
  }

  return 0;
}
