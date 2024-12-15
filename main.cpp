#include "./src/adventure_graph.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
  cout << "Program started\n";

  adventure_graph::build_graph();

  int window_length = adventure_graph::get_graph().size() - 1;
  int window_height = adventure_graph::get_graph()[0].size();

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(window_height, window_length),
                          "SFML Window");
  cout << "Window created\n";

  vector<pair<int, int>> skibidi = adventure_graph::get_coordinates();

  for (const auto &p : skibidi) {
    cout << "(" << p.first << ", " << p.second << ")" << endl;
  };
  // Get rooms (this function should return a vector of pairs of integers)
  vector<pair<int, int>> rooms = adventure_graph::get_rooms();
  vector<pair<int, int>> coords = adventure_graph::get_coordinates();

  // Create a vector of sf::Vector2i from the rooms
  vector<sf::Vector2i> sfmlRooms;

  // Convert each pair<int, int> to sf::Vector2i
  for (const auto &room : rooms) {
    sfmlRooms.push_back(sf::Vector2i(room.first, room.second));
  }

  // Create a rectangle and set its size based on the first room
  if (!sfmlRooms.empty()) { // Check if there are rooms in the list
    sf::RectangleShape rectangle;
    // Setting the rectangle's size based on the first room coordinates
    rectangle.setSize(
        sf::Vector2f(rooms[0].second,
                     rooms[0].first));         // Make sure these are reasonable
                                               // values for width and height
    rectangle.setFillColor(sf::Color::Blue);   // Fill color
    rectangle.setOutlineThickness(5);          // Outline thickness
    rectangle.setOutlineColor(sf::Color::Red); // Outline color
    rectangle.setPosition(coords[0].first, window_height - rooms[0].second);
    cout << "Height of room " << rooms[0].first << " \n";
    cout << window_height << " positon y \n";

    cout << window_height << endl;

    cout << coords[0].first << " first rooms x\n";
    cout << coords[0].second << " first rooms y\n";

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
          cout << "Window closing...\n";
          window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Escape) {
            cout << "Escape key pressed, closing window...\n";
            window.close();
          }
        }
      }

      // Clear the window and draw the rectangle
      window.clear(sf::Color::Black);
      window.draw(rectangle);
      window.display();
    }
  } else {
    cout << "No rooms found, unable to create rectangle.\n";
  }

  cout << "End of program\n";
  return 0;
}
