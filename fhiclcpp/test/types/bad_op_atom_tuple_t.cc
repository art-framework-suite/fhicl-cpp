#include "fhicl/types/Tuple.h"
#include "fhicl/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Tuple<int, long, double>> oa{1};
}
