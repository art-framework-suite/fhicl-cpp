#include "fhiclcpp/types/OptionalTable.h"
#include "fhiclcpp/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableAs<int>> ot{1};
}
