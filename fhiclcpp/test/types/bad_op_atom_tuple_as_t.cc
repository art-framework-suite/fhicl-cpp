#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/TupleAs.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<TupleAs<int, double, long>> oa{1};
}
