#include <bits/stdc++.h>
using namespace std;

namespace adventure_graph {

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist1(8, 20);
uniform_int_distribution<> dist2(0, 4);
uniform_int_distribution<> dist3(0, 1);
uniform_int_distribution<> dist6(0, 2);

vector<pair<int, int>> rooms, coords, paths;
vector<vector<char>> graph;

void print_coordinates_and_rooms() {
  for (int i = 0; i < rooms.size(); i++) {
    cout << "{x: " << coords[i].second << ", y: " << coords[i].first << ", "
         << "height: " << rooms[i].first << ", length: " << rooms[i].second
         << "}," << "\n";
  }

  cout << "\n\n\n";
}

void print_graph() {
  for (int i = graph.size() - 1; i > -1; i--) {
    cout << "\n";

    if (i < 10)
      cout << '0';

    cout << i << ':';

    for (int j = 0; j < graph[0].size(); j++)
      cout << graph[i][j];
  }

  cout << "\n";
}

void print_paths() {
  for (int i = 0; i < paths.size(); i++) {
    cout << paths[i].first + 1 << ":" << paths[i].second + 1 << "  ";
  }

  cout << "\n\n\n";
}

vector<pair<int, int>> get_paths() { return paths; }

vector<pair<int, int>> get_coordinates() { return coords; }

vector<pair<int, int>> get_rooms() { return rooms; }

vector<vector<char>> get_graph() { return graph; }

vector<vector<char>> put_rooms_to_graph(int maxRight, int maxUp) {
  vector<vector<char>> graph(maxRight, vector<char>(maxUp, ' '));

  for (size_t i = 0; i < rooms.size(); i++)
    for (int j = 0; j < rooms[i].first; j++)
      for (int k = 0; k < rooms[i].second; k++)
        graph[coords[i].first + j][coords[i].second + k] = '*';

  return graph;
}

auto get_formula(pair<float, float> middle1, pair<float, float> middle2,
                 int length) {
  int random = dist6(gen);

  // if (random == 1)
  // {

  return [middle1, middle2](int x) {
    return (middle2.first - middle1.first) / (middle2.second - middle1.second) *
           (x - middle1.second);
  };
  // }
  // else if (random)
  // {
  //     return [](int x)(0.1 * x / (length / 10) * (x - length) +
  //     middle1.first);
}

void draw_straight_line(auto formula, pair<float, float> middle1,
                        pair<float, float> middle2) {

  if (middle2.second == middle1.second) {
    for (size_t x = min(middle1.first, middle2.first);
         x <= max(middle1.first, middle2.first); x++) {
      graph[x][middle1.second] = 'X';
      graph[x][middle1.second - 1] = 'X';
    }
    return;
  }

  for (size_t x = min(middle1.second, middle2.second);
       x <= max(middle1.second, middle2.second); x++) {
    graph[middle1.first + formula(x)][x] = 'X';
    graph[middle1.first + formula(x)][x - 1] = 'X';

    if (formula(x) - formula(x - 1) > 1 &&
        x != min(middle1.second, middle2.second)) {
      for (size_t i = 0; i < formula(x) - formula(x - 1); i++) {
        graph[middle1.first + formula(x) - i][x] = 'X';
        graph[middle1.first + formula(x) - i][x - 1] = 'X';
      }
    } else if ((formula(x - 1) - formula(x) > 1) &&
               x != min(middle1.second, middle2.second)) {
      for (size_t i = 0; i < formula(x - 1) - formula(x); i++) {
        graph[middle1.first + formula(x) + i][x] = 'X';
        graph[middle1.first + formula(x) + i][x - 1] = 'X';
      }
    }
  }
}

void make_path(pair<float, float> middle1, pair<float, float> middle2,
               vector<vector<char>> &graph) {
  int length =
      hypot(middle1.first - middle2.first, middle2.second - middle1.second);

  auto formula = get_formula(middle1, middle2, length);

  draw_straight_line(formula, middle1, middle2);
}

void make_paths() {
  for (int i{}; i < paths.size(); i++) {
    int a = paths[i].first;
    int b = paths[i].second;

    pair<float, float> middle1 = {rooms[a].first / 2 + coords[a].first,
                                  rooms[a].second / 2 + coords[a].second};
    pair<float, float> middle2 = {rooms[b].first / 2 + coords[b].first,
                                  rooms[b].second / 2 + coords[b].second};

    make_path(middle1, middle2, graph);
  }

  cout << "\n";
}

pair<int, int> random_rectangle() { return {dist1(gen), dist1(gen)}; }

pair<int, int> random_coord(int maxUp, int maxRight) {
  uniform_int_distribution<> dist4(0, maxRight);
  uniform_int_distribution<> dist5(0, maxUp);

  return {dist5(gen), dist4(gen)};
}

bool scanRow(pair<int, int> rooms, pair<int, int> cords, pair<int, int> cord,
             pair<int, int> room, float j) {

  if (cords.second - 5 <= cord.first &&
      cords.first - 5 <= cord.second + room.first * (j / 4) &&
      cords.second + rooms.second + 5 >= cord.first &&
      cords.first + rooms.first + 5 >= cord.second + room.first * (j / 4))
    return true;
  if (cords.second - 5 <= cord.first + room.second / 4 &&
      cords.first - 5 <= cord.second + room.first * (j / 4) &&
      cords.second + rooms.second + 5 >= cord.first + room.second / 4 &&
      cords.first + rooms.first + 5 >= cord.second + room.first * (j / 4))
    return true;
  if (cords.second - 5 <= cord.first + room.second / 2 &&
      cords.first - 5 <= cord.second + room.first * (j / 4) &&
      cords.second + rooms.second + 5 >= cord.first + room.second / 2 &&
      cords.first + rooms.first + 5 >= cord.second + room.first * (j / 4))
    return true;
  if (cords.second - 5 <= cord.first + room.second * 0.77 &&
      cords.first - 5 <= cord.second + room.first * (j / 4) &&
      cords.second + rooms.second + 5 >= cord.first + room.second * 0.77 &&
      cords.first + rooms.first + 5 >= cord.second + room.first * (j / 4))
    return true;
  if (cords.second - 5 <= cord.first + room.second * (j / 4) &&
      cords.first - 5 <= cord.second + room.first &&
      cords.second + rooms.second + 5 >= cord.first + room.second * (j / 4) &&
      cords.first + rooms.first + 5 >= cord.second + room.first)
    return true;
  return false;
}

void place_rectangle(int &maxUp, int &maxRight, int size) {
  pair<int, int> room = random_rectangle();
  pair<int, int> cord;
  bool placeable = false;

  while (!placeable) {
    bool notTaken = false;
    bool inArea = false;

    vector<int> connected;

    cord = random_coord(maxUp, maxRight);

    for (size_t i = 0; i < rooms.size(); i++) {
      bool ifBreak = false;
      for (float j = 0; j < 4; j++) {
        if (scanRow(rooms[i], coords[i], cord, room, j)) {
          ifBreak = true;
          break;
        }
      }
      if (ifBreak)
        break;

      if (coords[i].first + rooms[i].first + 7 >= cord.first &&
          coords[i].first - 7 <= cord.first &&
          coords[i].second + rooms[i].second + 7 >= cord.second &&
          coords[i].second - 7 <= cord.second) {
        inArea = true;
        connected.push_back(i);
      } else if (coords[i].first + rooms[i].first + 7 >=
                     cord.first + room.second &&
                 coords[i].first - 7 <= cord.first + room.second &&
                 coords[i].second + rooms[i].second + 7 >= cord.second &&
                 coords[i].second - 7 <= cord.second) {
        inArea = true;
        connected.push_back(i);
      } else if (coords[i].first + rooms[i].first + 7 >= cord.first &&
                 coords[i].first - 7 <= cord.first &&
                 coords[i].second + rooms[i].second + 7 >=
                     cord.second + room.first &&
                 coords[i].second - 7 <= cord.second + room.first) {
        inArea = true;
        connected.push_back(i);
      } else if (coords[i].first + rooms[i].first + 7 >=
                     cord.first + room.second &&
                 coords[i].first - 7 <= cord.first + room.second &&
                 coords[i].second + rooms[i].second + 7 >=
                     cord.second + room.first &&
                 coords[i].second - 7 <= cord.second + room.first) {
        inArea = true;
        connected.push_back(i);
      }

      if (i == rooms.size() - 1) {
        notTaken = true;
      }
    }
    if (notTaken && inArea) {
      for (int i{}; i < connected.size(); i++) {
        paths.push_back({connected[i], size});
      }

      placeable = true;
      coords.push_back(cord);
      rooms.push_back(room);

      if (maxUp < room.second + cord.first + 7)
        maxUp = room.second + cord.first + 7;

      if (maxRight < room.first + cord.second + 7)
        maxRight = room.first + cord.second + 7;
    }
  }
}

void build_graph() {
  int amount = 9 - dist2(gen);

  rooms.push_back(random_rectangle());

  coords.push_back(pair<int, int>(0, 0));

  int maxUp{rooms[0].second + 7};
  int maxRight{rooms[0].first + 7};

  for (int i{}; i < amount - 1; i++) {
    place_rectangle(maxUp, maxRight, rooms.size());
  }

  graph = put_rooms_to_graph(maxUp, maxRight);

  make_paths();

  print_coordinates_and_rooms();
  print_paths();
  print_graph();
}
} // namespace adventure_graph
