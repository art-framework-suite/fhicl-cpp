#include "fhicl/types/OptionalAtom.h"
#include "fhicl/types/Sequence.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Sequence<int, 2>> oa{1};
}
