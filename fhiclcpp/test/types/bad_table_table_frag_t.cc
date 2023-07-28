#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<TableFragment<int>> t{Name{"Table"}, 1};
}
