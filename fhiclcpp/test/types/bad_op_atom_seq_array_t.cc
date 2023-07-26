#include "fhiclcpp/types/OptionalAtom.h"

#include <array>

using namespace fhicl;
int
main()
{
  [[maybe_unused]] OptionalAtom<std::array<int, 1>> oa{1};
}
