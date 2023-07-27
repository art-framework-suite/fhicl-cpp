#include "fhicl/types/Atom.h"
#include "fhicl/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Atom<int>> oa{1};
}
