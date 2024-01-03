#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalAtom.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalAtom<Atom<int>> oa{1};
}
