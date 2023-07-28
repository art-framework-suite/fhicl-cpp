#include "fhicl/types/OptionalAtom.h"
#include "fhicl/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Table<int>> oa{1};
}
