#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/Tuple.h"

using namespace fhicl;

int
main()
{
  [[maybe_unused]] Tuple<int, char, OptionalAtom<int>> t{Name{"Tuple"}};
}
