#include <SFML/Graphics.hpp>
#include <iostream>
#include "src/room.h"

void testGetSurroundWalls()
{
  // Test 1: Basic case with a room at (0, 0) and size (100, 200)
  Room room(0.0f, 0.0f, 200.0f, 100.0f);
  int wallWidth = 10;

  std::array<sf::Vector2f, 4> walls = room.getSurroundWalls(wallWidth);

  // Expected wall positions:
  sf::Vector2f expectedWall1(-10.0f, -10.0f);
  sf::Vector2f expectedWall2(-10.0f, 210.0f);
  sf::Vector2f expectedWall3(110.0f, 210.0f);
  sf::Vector2f expectedWall4(110.0f, -10.0f);

  // Check if the result matches the expected values
  if (walls[0] == expectedWall1 && walls[1] == expectedWall2 && walls[2] == expectedWall3 && walls[3] == expectedWall4)
  {
    std::cout << "Test 1 passed!" << std::endl;
  }
  else
  {
    std::cout << "Test 1 failed!" << std::endl;
  }

  // Test 2: Room positioned at (50, 50) with size (150, 250)
  Room room2(50.0f, 50.0f, 250.0f, 150.0f);
  wallWidth = 20;

  walls = room2.getSurroundWalls(wallWidth);

  // Expected wall positions:
  expectedWall1 = sf::Vector2f(30.0f, 30.0f);
  expectedWall2 = sf::Vector2f(30.0f, 320.0f);
  expectedWall3 = sf::Vector2f(200.0f, 320.0f);
  expectedWall4 = sf::Vector2f(200.0f, 30.0f);

  // Check if the result matches the expected values
  if (walls[0] == expectedWall1 && walls[1] == expectedWall2 && walls[2] == expectedWall3 && walls[3] == expectedWall4)
  {
    std::cout << "Test 2 passed!" << std::endl;
  }
  else
  {
    std::cout << "Test 2 failed!" << std::endl;
  }
}

int main()
{
  // Run the test
  testGetSurroundWalls();

  return 0;
}
