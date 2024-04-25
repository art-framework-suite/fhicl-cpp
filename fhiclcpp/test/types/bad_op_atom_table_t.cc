#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/Table.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Table<int>> oa{1};
}
