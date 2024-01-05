#include "fhiclcpp/types/OptionalAtom.h"

#include <tuple>

int
main()
{
  [[maybe_unused]] fhicl::OptionalAtom<std::tuple<int, long, double>> oa{1};
}
