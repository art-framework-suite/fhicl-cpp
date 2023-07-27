#include "fhicl/types/Table.h"
#include "fhicl/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Table<int>> oa{1};
}
