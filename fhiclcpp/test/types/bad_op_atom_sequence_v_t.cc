#include "fhicl/types/Sequence.h"
#include "fhicl/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Sequence<int>> oa{1};
}
