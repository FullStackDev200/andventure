#include <bits/stdc++.h>
using namespace std;

namespace creature
{
  class creature
  {
   private:
    pair<int, int> location;

   public:
    pair<int, int> get_location()
    {
      return location;
    }
    pair<int, int> change_location(pair<int, int> location_new)
    {
      location = location_new;
    }
  };
}  // namespace creature