#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist1(8, 20);
uniform_int_distribution<> dist2(0, 4);
uniform_int_distribution<> dist3(0, 1);
uniform_int_distribution<> dist6(0, 2);

void printCords(vector<pair<int, int>> rooms, vector<pair<int, int>> cords)
{
  for (int i = 0; i < rooms.size(); i++)
  {
    cout << "{x: " << cords[i].second << ", y: " << cords[i].first << ", " << "height: " << rooms[i].first << ", length: " << rooms[i].second << "}," << "\n";
  }

  cout << "\n"
    << "\n"
    << "\n";
}

void printMap(vector<vector<char>> map)
{
  for (int i = map.size() - 1; i > -1; i--)
  {
    cout << "\n";

    if (i < 10)
      cout << '0';

    cout << i << ':';

    for (int j = 0; j < map[0].size(); j++)
      cout << map[i][j];
  }

  cout << "\n";
}

void printPaths(vector<pair<int, int>> paths)
{
  for (int i = 0; i < paths.size(); i++)
  {
    cout << paths[i].first + 1 << ":" << paths[i].second + 1 << "  ";
  }

  cout << "\n"
    << "\n"
    << "\n";
}

vector<vector<char>> putRoomsToMap(vector<pair<int, int>> rooms, vector<pair<int, int>> cords, int maxRight, int maxUp)
{
  vector<vector<char>> map(maxRight, vector<char>(maxUp, ' '));

  for (size_t i = 0; i < rooms.size(); i++)
    for (int j = 0; j < rooms[i].first; j++)
      for (int k = 0; k < rooms[i].second; k++)
        map[cords[i].first + j][cords[i].second + k] = '*';

  return map;
}

auto getFormula(pair<float, float> middle1, pair<float, float> middle2, int length)
{
  int random = dist6(gen);

  // if (random == 1)
  // {

  return [middle1, middle2](int x)
  {
    return (middle2.first - middle1.first) / (middle2.second - middle1.second) * (x - middle1.second);
  };
  // }
  // else if (random)
  // {
  //     return [](int x)(0.1 * x / (length / 10) * (x - length) + middle1.first);
}

void drawStraightLine(auto formula, vector<vector<char>> &map, pair<float, float> middle1, pair<float, float> middle2)
{

  if (middle2.second == middle1.second)
  {
    for (size_t x = min(middle1.first, middle2.first); x <= max(middle1.first, middle2.first); x++)
    {
      map[x][middle1.second] = 'X';
      map[x][middle1.second - 1] = 'X';
    }
    return;
  }

  for (size_t x = min(middle1.second, middle2.second); x <= max(middle1.second, middle2.second); x++)
  {
    map[middle1.first + formula(x)][x] = 'X';
    map[middle1.first + formula(x)][x - 1] = 'X';

    if (formula(x) - formula(x - 1) > 1 && x != min(middle1.second, middle2.second))
    {
      for (size_t i = 0; i < formula(x) - formula(x - 1); i++)
      {
        map[middle1.first + formula(x) - i][x] = 'X';
        map[middle1.first + formula(x) - i][x - 1] = 'X';
      }
    }
    else if ((formula(x - 1) - formula(x) > 1) && x != min(middle1.second, middle2.second))
    {
      for (size_t i = 0; i < formula(x - 1) - formula(x); i++)
      {
        map[middle1.first + formula(x) + i][x] = 'X';
        map[middle1.first + formula(x) + i][x - 1] = 'X';
      }
    }
  }
}

void makePath(pair<float, float> middle1, pair<float, float> middle2, vector<vector<char>> &map)
{
  int length = sqrt((middle2.first - middle1.first) * (middle2.first - middle1.first)) + (middle2.second - middle1.second) * ((middle2.second - middle1.second));

  auto formula = getFormula(middle1, middle2, length);

  drawStraightLine(formula, map, middle1, middle2);
}

void makePaths(vector<pair<int, int>> rooms, vector<pair<int, int>> cords, vector<pair<int, int>> paths, vector<vector<char>> &map)
{
  for (int i{}; i < paths.size(); i++)
  {
    int a = paths[i].first;
    int b = paths[i].second;

    pair<float, float> middle1 = {rooms[a].first / 2 + cords[a].first, rooms[a].second / 2 + cords[a].second};
    pair<float, float> middle2 = {rooms[b].first / 2 + cords[b].first, rooms[b].second / 2 + cords[b].second};

    makePath(middle1, middle2, map);
  }

  cout << "\n";
}

pair<int, int> randomRectangle()
{
  return {dist1(gen), dist1(gen)};
}

pair<int, int> randomCord(int maxUp, int maxRight)
{
  uniform_int_distribution<> dist4(0, maxRight);
  uniform_int_distribution<> dist5(0, maxUp);

  return {dist5(gen), dist4(gen)};
}

bool scanRow(pair<int, int> rooms, pair<int, int> cords, pair<int, int> cord, pair<int, int> room, float j)
{

  if (cords.first - 5 <= cord.first && cords.second - 5 <= cord.second + room.second * (j / 4) && cords.first + rooms.first + 5 >= cord.first && cords.second + rooms.second + 5 >= cord.second + room.second * (j / 4))
    return true;
  if (cords.first - 5 <= cord.first + room.first / 4 && cords.second - 5 <= cord.second + room.second * (j / 4) && cords.first + rooms.first + 5 >= cord.first + room.first / 4 && cords.second + rooms.second + 5 >= cord.second + room.second * (j / 4))
    return true;
  if (cords.first - 5 <= cord.first + room.first / 2 && cords.second - 5 <= cord.second + room.second * (j / 4) && cords.first + rooms.first + 5 >= cord.first + room.first / 2 && cords.second + rooms.second + 5 >= cord.second + room.second * (j / 4))
    return true;
  if (cords.first - 5 <= cord.first + room.first * 0.77 && cords.second - 5 <= cord.second + room.second * (j / 4) && cords.first + rooms.first + 5 >= cord.first + room.first * 0.77 && cords.second + rooms.second + 5 >= cord.second + room.second * (j / 4))
    return true;
  if (cords.first - 5 <= cord.first + room.first * (j / 4) && cords.second - 5 <= cord.second + room.second && cords.first + rooms.first + 5 >= cord.first + room.first * (j / 4) && cords.second + rooms.second + 5 >= cord.second + room.second)
    return true;
  return false;
}

void placeRectangle(vector<pair<int, int>> &rooms, vector<pair<int, int>> &cords, vector<pair<int, int>> &paths, int &maxUp, int &maxRight, int size)
{
  pair<int, int> room = randomRectangle();
  pair<int, int> cord;
  bool placeable = false;

  while (!placeable)
  {
    bool notTaken = false;
    bool inArea = false;

    vector<int> connected;

    cord = randomCord(maxUp, maxRight);

    for (size_t i = 0; i < rooms.size(); i++)
    {
      bool ifBreak = false;
      for (float j = 0; j < 4; j++)
      {
        if (scanRow(rooms[i], cords[i], cord, room, j))
        {
          ifBreak = true;
          break;
        }
      }
      if (ifBreak)
        break;

      if (cords[i].first + rooms[i].first + 7 >= cord.first && cords[i].first - 7 <= cord.first && cords[i].second + rooms[i].second + 7 >= cord.second && cords[i].second - 7 <= cord.second)
      {
        inArea = true;
        connected.push_back(i);
      }
      else if (cords[i].first + rooms[i].first + 7 >= cord.first + room.first && cords[i].first - 7 <= cord.first + room.first && cords[i].second + rooms[i].second + 7 >= cord.second && cords[i].second - 7 <= cord.second)
      {
        inArea = true;
        connected.push_back(i);
      }
      else if (cords[i].first + rooms[i].first + 7 >= cord.first && cords[i].first - 7 <= cord.first && cords[i].second + rooms[i].second + 7 >= cord.second + room.second && cords[i].second - 7 <= cord.second + room.second)
      {
        inArea = true;
        connected.push_back(i);
      }
      else if (cords[i].first + rooms[i].first + 7 >= cord.first + room.first && cords[i].first - 7 <= cord.first + room.first && cords[i].second + rooms[i].second + 7 >= cord.second + room.second && cords[i].second - 7 <= cord.second + room.second)
      {
        inArea = true;
        connected.push_back(i);
      }

      if (i == rooms.size() - 1)
      {
        notTaken = true;
      }
    }
    if (notTaken && inArea)
    {
      for (int i{}; i < connected.size(); i++)
      {
        paths.push_back({connected[i], size});
      }

      placeable = true;
      cords.push_back(cord);
      rooms.push_back(room);

      if (maxUp < room.first + cord.first + 7)
        maxUp = room.first + cord.first + 7;

      if (maxRight < room.second + cord.second + 7)
        maxRight = room.second + cord.second + 7;
    }
  }
}

vector<vector<char>> BuildRandomRooms(int amount)
{
  vector<pair<int, int>> rooms;

  vector<pair<int, int>> cords;

  vector<pair<int, int>> paths;

  rooms.push_back(randomRectangle());

  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(100, 50));
  cords.push_back(pair<int, int>(0, 0));

  int maxUp{rooms[0].second + 7};
  int maxRight{rooms[0].first + 7};

  for (int i{}; i < amount - 1; i++)
  {
    placeRectangle(rooms, cords, paths, maxUp, maxRight, rooms.size());
  }

  vector<vector<char>> map = putRoomsToMap(rooms, cords, maxUp, maxRight);

  makePaths(rooms, cords, paths, map);

  printCords(rooms, cords);
  printPaths(paths);
  printMap(map);

  return map;
}
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf
//d/fasd/fsdfasf

vector<vector<char>> buildMap()
{
  vector<vector<char>> map =  BuildRandomRooms(9 - dist2(gen));

  return map;
}

int main()
{


  vector<vector<char>> map = buildMap();

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
