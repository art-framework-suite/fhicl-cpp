#include "fhiclcpp/types/Table.h"
#include <vector>

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<std::vector<int>> t{Name{"Table"}, 1};
}
