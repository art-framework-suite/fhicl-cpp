#include "fhicl/types/TableAs.h"
#include "fhicl/types/OptionalTable.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableAs<int>> ot{1};
}