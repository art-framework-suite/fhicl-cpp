#include "fhicl/types/OptionalTable.h"
#include "fhicl/types/TableFragment.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableFragment<int>> ot{1};
}
