#include "fhicl/types/TableFragment.h"
#include "fhicl/types/OptionalTable.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableFragment<int>> ot{1};
}
