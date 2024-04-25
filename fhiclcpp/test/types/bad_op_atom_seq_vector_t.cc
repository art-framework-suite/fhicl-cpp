#include "fhiclcpp/types/OptionalAtom.h"

#include <vector>

int
main()
{
  [[maybe_unused]] fhicl::OptionalAtom<std::vector<int>> oa{1};
}
