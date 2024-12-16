#include "./src/adventure_graph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace adventure_graph;

int main() {

  build_graph();

  vector<pair<int, int>> rooms = get_rooms();
  vector<pair<int, int>> coords = get_coordinates();

  int window_length = (get_graph().size() - 1);
  int window_height = (get_graph()[0].size());

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_length, window_height),
                          "SFML Window");

  /*std::vector<sf::RectangleShape> rectangles;*/
  /*for (int i = 0; i < rooms.size(); i++) {*/
  /**/
  /*  sf::RectangleShape rectangle;*/
  /**/
  /*  rectangle.setSize(*/
  /*      sf::Vector2f(rooms[i].first,*/
  /*                   rooms[i].second));        // Make sure these are
   * reasonable*/
  /*                                             // values for width and
   * height*/
  /*  rectangle.setFillColor(sf::Color::Blue);   // Fill color*/
  /*  rectangle.setOutlineThickness(5);          // Outline thickness*/
  /*  rectangle.setOutlineColor(sf::Color::Red); // Outline color*/
  /*  rectangle.setPosition(rooms[i].first, window_height - rooms[i].second);*/
  /*}*/

  // Check if there are rooms in the list
  sf::RectangleShape rectangle;
  // Setting the rectangle's size based on the second room coordinates
  rectangle.setSize(sf::Vector2f(rooms[0].first,
                                 rooms[0].second)); // Make sure these a

  // values for width and height
  rectangle.setFillColor(sf::Color::Blue);   // Fill color
  rectangle.setOutlineThickness(5);          // Outline thickness
  rectangle.setOutlineColor(sf::Color::Red); // Outline color
  rectangle.setPosition(0, window_height - rooms[0].second);

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
    window.draw(rectangle);
    window.display();
  }

  return 0;
}
