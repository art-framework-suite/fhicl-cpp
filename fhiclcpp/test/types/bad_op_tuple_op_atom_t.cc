#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/OptionalTuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] OptionalTuple<int, char, OptionalAtom<int>> ot{Name{"OptionalTuple"}};
}
