#include "src/game.hpp"

int main()
{
  Game game;
  game.loadMap();
  game.preRenderMap();
  game.render();
  bool gameShouldClose = false;
  while (!gameShouldClose)
  {
    game.run();
  }
}
