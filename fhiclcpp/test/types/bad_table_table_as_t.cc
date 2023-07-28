#include "fhiclcpp/types/Table.h"
#include "fhiclcpp/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Table<TableAs<int>> t{Name{"Table"}, 1};
}
