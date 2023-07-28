#include "fhicl/types/OptionalTable.h"
#include "fhicl/types/TableAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTable<TableAs<int>> ot{1};
}
