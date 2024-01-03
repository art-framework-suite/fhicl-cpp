#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableFragment<int>> ot{1};
}
