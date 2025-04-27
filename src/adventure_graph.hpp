#ifndef ROOM_COORDS
#define ROOM_COORDS
#include <vector>
#include <utility>

using namespace std;

namespace adventure_graph {
    void build_graph();

    void print_coordinates_and_rooms();
    void print_graph();
    void print_paths();
    void print_rooms();

    vector<pair<int, int>> get_paths();
    vector<pair<int, int>> get_coordinates();
    vector<pair<int, int>> get_rooms();
    vector<pair<pair<int,int>,pair<int,int>>> get_middles();

    vector<vector<char>> get_graph();

    pair<int, int> get_location();
    pair<int,int> change_location(pair<int,int> location_new);
}

#endif // ROOM_COORDS