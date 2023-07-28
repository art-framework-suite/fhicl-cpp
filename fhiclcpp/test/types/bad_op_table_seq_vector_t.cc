#include "fhiclcpp/types/OptionalTable.h"
#include <vector>

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<vector<int>> ot{Name{"OptionalTable"}, 1};
}
