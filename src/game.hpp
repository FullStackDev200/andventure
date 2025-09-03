#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "path.h"
#include "player.h"
#include "room.h"
#include "sfml_helpers.hpp"

class Game
{
 public:
  Game();
  int run();

  void loadMap();
  void render();
  void preRenderMap();

 private:
  void processEvents();
  void update(float dt);
  void handleCollisions();

  void generatePaths();
  void makePathWalls();

  sf::RenderWindow window;
  sf::RenderTexture renderTexture;
  sf::Sprite staticBackground;

  std::vector<std::array<sf::Vector2f, 3>> pathsCoords;

  std::vector<Room> rooms;
  std::vector<Path> paths;
  std::vector<sf::FloatRect> doors;
  std::vector<sf::RectangleShape> walls;
  Player player;
  int scale;
};
