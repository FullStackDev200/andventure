#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "./src/adventure_graph.hpp";

using namespace std;
using namespace adventure_graph;

int main()
{
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(100, 50));
  
  build_graph();

  vector<vector<char>> graph = get_graph();

  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
  window.setVerticalSyncEnabled(false);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    } 

    window.clear();
    
    sf::RectangleShape room;
    room.setSize(sf::Vector2f(100, 50));
    room.setOutlineColor(sf::Color::Red);
    room.setOutlineThickness(5);
    room.setPosition(10, 20);
    window.draw(room);

    window.display();
  }

  cout << "end";
}
